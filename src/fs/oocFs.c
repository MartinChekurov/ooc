#include "oocFs.h"
#include "oocFs.r"
#include "oocAbstractBlockDevice.h"
#include "oocError.h"
#include "oocModifiers.h"
#include "oocObject.h"
#include "oocObject.r"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static OOC_FsClass* FsClass;
static OOC_FsClass  FsClassInstance;

static OOC_Error ooc_fsWriteSuperblock(OOC_Fs* fs) {
    uint8_t* buf = calloc(1, fs->superblock.blockSize);
    if (!buf) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    memcpy(buf, &fs->superblock, sizeof(fs->superblock));
    OOC_Error err = ooc_abstractBlockDeviceWrite(fs->device, 0, buf);
    free(buf);
    return err;
}

static uint32_t ooc_fsGetBitmapBlockIndex(OOC_Fs* fs, uint32_t index) {
    uint32_t bitsPerBlock = (uint32_t)(fs->superblock.blockSize * 8);
    return index / bitsPerBlock;
}

static uint32_t ooc_fsGetBitmapBitOffset(OOC_Fs* fs, uint32_t index) {
    uint32_t bitsPerBlock = (uint32_t)(fs->superblock.blockSize * 8);
    return index % bitsPerBlock;
}

static void* ooc_fsReadBitmapBlock(OOC_Fs* fs, uint32_t bitmapStart, uint32_t totalBits, uint32_t index) {
    if (index >= totalBits) {
        return NULL;
    }
    uint8_t* buf = malloc(fs->superblock.blockSize);
    if (!buf) {
        return NULL;
    }
    uint32_t absBlock = bitmapStart + ooc_fsGetBitmapBlockIndex(fs, index);
    if (ooc_abstractBlockDeviceRead(fs->device, absBlock, buf) != OOC_ERROR_NONE) {
        free(buf);
        return NULL;
    }
    return buf;
}

static OOC_Error ooc_fsWriteBitmapBlock(OOC_Fs* fs, uint32_t bitmapStart, uint32_t totalBits, uint32_t index, const void* block) {
    if (index >= totalBits) {
        return OOC_ERROR_OUT_OF_BOUNDS;
    }
    uint32_t absBlock = bitmapStart + ooc_fsGetBitmapBlockIndex(fs, index);
    return ooc_abstractBlockDeviceWrite(fs->device, absBlock, block);
}

static OOC_Error ooc_fsSetBitInBlock(void* block, uint32_t bitOffset) {
    ((uint8_t*)block)[bitOffset / 8] |= (uint8_t)(1u << (bitOffset % 8));
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_fsClearBitInBlock(void* block, uint32_t bitOffset) {
    ((uint8_t*)block)[bitOffset / 8] &= (uint8_t)~(1u << (bitOffset % 8));
    return OOC_ERROR_NONE;
}

static bool ooc_fsGetBitFromBlock(const void* block, uint32_t bitOffset) {
    return (((const uint8_t*)block)[bitOffset / 8] >> (bitOffset % 8)) & 1;
}

static OOC_Error ooc_fsClearBitmapBit(OOC_Fs* fs, uint32_t bitmapStart, uint32_t totalBits, uint32_t index) {
    void* block = ooc_fsReadBitmapBlock(fs, bitmapStart, totalBits, index);
    if (!block) {
        return (index >= totalBits) ? OOC_ERROR_OUT_OF_BOUNDS : OOC_ERROR_OUT_OF_MEMORY;
    }
    uint32_t bitOffset = ooc_fsGetBitmapBitOffset(fs, index);
    if (!ooc_fsGetBitFromBlock(block, bitOffset)) {
        free(block);
        return OOC_ERROR_NOT_FOUND;
    }
    ooc_fsClearBitInBlock(block, bitOffset);
    OOC_Error err = ooc_fsWriteBitmapBlock(fs, bitmapStart, totalBits, index, block);
    free(block);
    return err;
}

static OOC_Error ooc_fsSetBitmapBit(OOC_Fs* fs, uint32_t bitmapStart, uint32_t totalBits, uint32_t index) {
    void* block = ooc_fsReadBitmapBlock(fs, bitmapStart, totalBits, index);
    if (!block) {
        return (index >= totalBits) ? OOC_ERROR_OUT_OF_BOUNDS : OOC_ERROR_OUT_OF_MEMORY;
    }
    uint32_t bitOffset = ooc_fsGetBitmapBitOffset(fs, index);
    if (ooc_fsGetBitFromBlock(block, bitOffset)) {
        free(block);
        return OOC_ERROR_DUPLICATE_FOUND;
    }
    ooc_fsSetBitInBlock(block, bitOffset);
    OOC_Error err = ooc_fsWriteBitmapBlock(fs, bitmapStart, totalBits, index, block);
    free(block);
    return err;
}

static bool ooc_fsGetBitmapBit(OOC_Fs* fs, uint32_t bitmapStart, uint32_t totalBits, uint32_t index) {
    void* block = ooc_fsReadBitmapBlock(fs, bitmapStart, totalBits, index);
    if (!block) {
        return false;
    }
    bool isSet = ooc_fsGetBitFromBlock(block, ooc_fsGetBitmapBitOffset(fs, index));
    free(block);
    return isSet;
}

static OOC_Error ooc_fsClearInodeBit(OOC_Fs* fs, uint32_t inodeNum) {
    return ooc_fsClearBitmapBit(fs, fs->superblock.inodeBitmapBlock, fs->superblock.totalInodes, inodeNum);
}

static OOC_Error ooc_fsSetInodeBit(OOC_Fs* fs, uint32_t inodeNum) {
    return ooc_fsSetBitmapBit(fs, fs->superblock.inodeBitmapBlock, fs->superblock.totalInodes, inodeNum);
}

static bool ooc_fsIsInodeBitSet(OOC_Fs* fs, uint32_t inodeNum) {
    return ooc_fsGetBitmapBit(fs, fs->superblock.inodeBitmapBlock, fs->superblock.totalInodes, inodeNum);
}

static OOC_Error ooc_fsClearDataBit(OOC_Fs* fs, uint32_t dataBlockIndex) {
    return ooc_fsClearBitmapBit(fs, fs->superblock.dataBitmapBlock, fs->superblock.dataCount, dataBlockIndex);
}

static OOC_Error ooc_fsSetDataBit(OOC_Fs* fs, uint32_t dataBlockIndex) {
    return ooc_fsSetBitmapBit(fs, fs->superblock.dataBitmapBlock, fs->superblock.dataCount, dataBlockIndex);
}

static bool ooc_fsIsDataBitSet(OOC_Fs* fs, uint32_t dataBlockIndex) {
    return ooc_fsGetBitmapBit(fs, fs->superblock.dataBitmapBlock, fs->superblock.dataCount, dataBlockIndex);
}

static OOC_Error ooc_fsFindFreeBit(OOC_Fs* fs, uint32_t bitmapStart, uint32_t totalBits, uint32_t* outIndex) {
    uint32_t blockSize    = fs->superblock.blockSize;
    uint32_t bitsPerBlock = blockSize * 8;
    uint32_t bitmapCount  = (totalBits + bitsPerBlock - 1) / bitsPerBlock;
    uint8_t* buf = malloc(blockSize);
    if (!buf) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    uint32_t bit = 0;
    for (uint32_t b = 0; b < bitmapCount; b++) {
        OOC_Error err = ooc_abstractBlockDeviceRead(fs->device, bitmapStart + b, buf);
        if (err != OOC_ERROR_NONE) {
            free(buf);
            return err; 
        }
        for (uint32_t i = 0; i < bitsPerBlock && bit < totalBits; i++, bit++) {
            if (!ooc_fsGetBitFromBlock(buf, i)) {
                free(buf);
                *outIndex = bit;
                return OOC_ERROR_NONE;
            }
        }
    }
    free(buf);
    return OOC_ERROR_NO_SPACE;
}

static OOC_Error ooc_fsGetFreeInode(OOC_Fs* fs, uint32_t* outInodeNum) {
    return ooc_fsFindFreeBit(fs, fs->superblock.inodeBitmapBlock, fs->superblock.totalInodes, outInodeNum);
}

static OOC_Error ooc_fsGetFreeDataBlock(OOC_Fs* fs, uint32_t* outBlockIndex) {
    return ooc_fsFindFreeBit(fs, fs->superblock.dataBitmapBlock, fs->superblock.dataCount, outBlockIndex);
}

static uint32_t ooc_fsGetInodeBlockIndex(OOC_Fs* fs, uint32_t inodeNum) {
    uint32_t inodesPerBlock = (uint32_t)(fs->superblock.blockSize / sizeof(OOC_FsInodeDisk));
    return inodeNum / inodesPerBlock;
}

static uint32_t ooc_fsGetInodeIndexInBlock(OOC_Fs* fs, uint32_t inodeNum) {
    uint32_t inodesPerBlock = (uint32_t)(fs->superblock.blockSize / sizeof(OOC_FsInodeDisk));
    return inodeNum % inodesPerBlock;
}

static uint32_t ooc_fsGetInodeOffsetInBlock(OOC_Fs* fs, uint32_t inodeNum) {
    return ooc_fsGetInodeIndexInBlock(fs, inodeNum) * (uint32_t)sizeof(OOC_FsInodeDisk);
}

static void* ooc_fsReadInodeBlock(OOC_Fs* fs, uint32_t inodeNum) {
    if (!ooc_fsIsInodeBitSet(fs, inodeNum)) {
        return NULL;
    }
    uint8_t* buf = malloc(fs->superblock.blockSize);
    if (!buf) {
        return NULL;
    }
    uint32_t absBlock = fs->superblock.inodeTableBlock + ooc_fsGetInodeBlockIndex(fs, inodeNum);
    if (ooc_abstractBlockDeviceRead(fs->device, absBlock, buf) != OOC_ERROR_NONE) {
        free(buf);
        return NULL;
    }
    return buf;
}

static OOC_Error ooc_fsReadInodeFromBlock(OOC_Fs* fs, uint32_t inodeNum, const void* block, OOC_FsInodeDisk* inode) {
    uint32_t byteOffset = ooc_fsGetInodeOffsetInBlock(fs, inodeNum);
    memcpy(inode, (const uint8_t*)block + byteOffset, sizeof(OOC_FsInodeDisk));
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_fsReadInode(OOC_Fs* fs, uint32_t inodeNum, OOC_FsInodeDisk* inode) {
    void* block = ooc_fsReadInodeBlock(fs, inodeNum);
    if (!block) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    OOC_Error err = ooc_fsReadInodeFromBlock(fs, inodeNum, block, inode);
    free(block);
    return err;
}

static OOC_Error ooc_fsWriteInodeToBlock(OOC_Fs* fs, uint32_t inodeNum, void* block, const OOC_FsInodeDisk* inode) {
    uint32_t byteOffset = ooc_fsGetInodeOffsetInBlock(fs, inodeNum);
    memcpy((uint8_t*)block + byteOffset, inode, sizeof(OOC_FsInodeDisk));
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_fsWriteInodeBlock(OOC_Fs* fs, uint32_t inodeNum, const void* block) {
    uint32_t absBlock = fs->superblock.inodeTableBlock + ooc_fsGetInodeBlockIndex(fs, inodeNum);
    return ooc_abstractBlockDeviceWrite(fs->device, absBlock, block);
}

static OOC_Error ooc_fsWriteInode(OOC_Fs* fs, uint32_t inodeNum, const OOC_FsInodeDisk* inode) {
    void* block = ooc_fsReadInodeBlock(fs, inodeNum);
    if (!block) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    OOC_Error err = ooc_fsWriteInodeToBlock(fs, inodeNum, block, inode);
    if (err != OOC_ERROR_NONE) {
        free(block);
        return err;
    }
    err = ooc_fsWriteInodeBlock(fs, inodeNum, block);
    free(block);
    return err;
}

/* Reserve an inode number (find free, set bitmap, update superblock)
 * without writing the inode data — caller does that separately. */
static OOC_Error ooc_fsAllocInode(OOC_Fs* fs, uint32_t* outInodeNum) {
    OOC_Error err = ooc_fsGetFreeInode(fs, outInodeNum);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    err = ooc_fsSetInodeBit(fs, *outInodeNum);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    fs->superblock.freeInodes--;
    return ooc_fsWriteSuperblock(fs);
}

static OOC_Error ooc_fsAddInode(OOC_Fs* fs, const OOC_FsInodeDisk* inode, uint32_t* outInodeNum) {
    OOC_Error err = ooc_fsAllocInode(fs, outInodeNum);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    return ooc_fsWriteInode(fs, *outInodeNum, inode);
}

static OOC_Error ooc_fsDeleteInode(OOC_Fs* fs, uint32_t inodeNum) {
    OOC_Error err = ooc_fsClearInodeBit(fs, inodeNum);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    fs->superblock.freeInodes++;
    return ooc_fsWriteSuperblock(fs);
}

static OOC_Error ooc_fsReadDataBlock(OOC_Fs* fs, uint32_t blockIndex, void* buf) {
    if (!ooc_fsIsDataBitSet(fs, blockIndex)) {
        return OOC_ERROR_NOT_FOUND;
    }
    return ooc_abstractBlockDeviceRead(fs->device, fs->superblock.dataBlock + blockIndex, buf);
}

static OOC_Error ooc_fsWriteDataBlock(OOC_Fs* fs, uint32_t blockIndex, const void* buf) {
    if (!ooc_fsIsDataBitSet(fs, blockIndex)) {
        return OOC_ERROR_NOT_FOUND;
    }
    return ooc_abstractBlockDeviceWrite(fs->device, fs->superblock.dataBlock + blockIndex, buf);
}

static OOC_Error ooc_fsAddDataBlock(OOC_Fs* fs, const void* buf, uint32_t* outBlockIndex) {
    OOC_Error err = ooc_fsGetFreeDataBlock(fs, outBlockIndex);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    err = ooc_fsSetDataBit(fs, *outBlockIndex);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    fs->superblock.freeBlocks--;
    err = ooc_fsWriteSuperblock(fs);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    return ooc_fsWriteDataBlock(fs, *outBlockIndex, buf);
}

static OOC_Error ooc_fsDeleteDataBlock(OOC_Fs* fs, uint32_t blockIndex) {
    OOC_Error err = ooc_fsClearDataBit(fs, blockIndex);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    fs->superblock.freeBlocks++;
    return ooc_fsWriteSuperblock(fs);
}

static uint32_t ooc_fsGetPtrsPerBlock(OOC_Fs* fs) {
    return fs->superblock.blockSize / (uint32_t)sizeof(uint32_t);
}

static OOC_FsFile* ooc_fsAllocFileHandle(uint32_t inodeNum) {
    OOC_FsFile* f = malloc(sizeof(OOC_FsFile));
    if (f) {
        f->inodeNum = inodeNum;
    }
    return f;
}

void ooc_fsFileClose(OOC_FsFile* file) {
    free(file);
}

typedef enum {
    OOC_FS_WALK_DATA,
    OOC_FS_WALK_TABLE,
} OOC_FsWalkEvent;

typedef struct OOC_FsWalk OOC_FsWalk;

typedef struct OOC_FsBlockVisit {
    OOC_FsWalkEvent  event;
    uint32_t         logicalIndex;
    uint32_t*        slot;            /* pointer to this slot in the walker's in-memory buffer */
    uint32_t         containingBlock; /* physical address of the block holding this slot; 0 = inode */
    void*            containingBuf;   /* full in-memory buffer of containingBlock; NULL if inode */
} OOC_FsBlockVisit;

typedef OOC_Error (*OOC_FsBlockVisitor)(OOC_FsWalk* walk, OOC_FsBlockVisit* visit);

struct OOC_FsWalk {
    OOC_Fs*            fs;
    OOC_FsInodeDisk*   inode;
    uint32_t           inodeNum;
    OOC_FsBlockVisitor visitor;
    void*              ctx;
    /* internal traversal state */
    uint32_t           logicalBase;
    uint32_t*          slotPtr;
    uint32_t           containingBlock;
    void*              containingBuf;
    uint32_t           level;
};

static OOC_Error ooc_fsWalkIndirect(OOC_FsWalk* walk) {
    OOC_Fs* fs = walk->fs;
    uint32_t ptrsPerBlock = ooc_fsGetPtrsPerBlock(fs);

    /* Capture traversal state as locals before emitting the TABLE event
     * (visitor may modify the struct) and before recursion overwrites fields. */
    uint32_t  level          = walk->level;
    uint32_t* slotPtr        = walk->slotPtr;
    uint32_t  containingBlock = walk->containingBlock;
    void*     containingBuf   = walk->containingBuf;

    OOC_FsBlockVisit selfVisit = {
        .event           = OOC_FS_WALK_TABLE,
        .logicalIndex    = walk->logicalBase,
        .slot            = slotPtr,
        .containingBlock = containingBlock,
        .containingBuf   = containingBuf,
    };
    OOC_Error err = walk->visitor(walk, &selfVisit);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    uint32_t tableAbsBlock = *slotPtr;
    if (tableAbsBlock == 0) {
        uint32_t subtreeSize = 1;
        for (uint32_t l = 0; l <= level; l++) {
            subtreeSize *= ptrsPerBlock;
        }
        walk->logicalBase += subtreeSize;
        return OOC_ERROR_NONE;
    }
    uint32_t* buf = malloc(fs->superblock.blockSize);
    if (!buf) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    err = ooc_abstractBlockDeviceRead(fs->device, tableAbsBlock, buf);
    if (err != OOC_ERROR_NONE) {
        free(buf);
        return err;
    }
    for (uint32_t i = 0; i < ptrsPerBlock; i++) {
        if (level == 0) {
            if (buf[i] != 0) {
                OOC_FsBlockVisit visit = {
                    .event           = OOC_FS_WALK_DATA,
                    .logicalIndex    = walk->logicalBase,
                    .slot            = &buf[i],
                    .containingBlock = tableAbsBlock,
                    .containingBuf   = buf,
                };
                err = walk->visitor(walk, &visit);
                if (err != OOC_ERROR_NONE) {
                    break;
                }
            }
            walk->logicalBase++;
        } else {
            walk->slotPtr        = &buf[i];
            walk->containingBlock = tableAbsBlock;
            walk->containingBuf   = buf;
            walk->level           = level - 1;
            err = ooc_fsWalkIndirect(walk);
            if (err != OOC_ERROR_NONE) {
                break;
            }
        }
    }
    free(buf);
    return err;
}

static OOC_Error ooc_fsWalkBlocks(OOC_FsWalk* walk) {
    if (!walk) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    if (!walk->inode) {
        return OOC_ERROR_NULL_POINTER;
    }
    if (!walk->visitor) {
        return OOC_ERROR_NULL_POINTER;
    }
    if (!walk->fs) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_FsInodeDisk* inode = walk->inode;
    walk->logicalBase = 0;
    for (uint32_t i = 0; i < OOC_FS_DIRECT_BLOCKS; i++) {
        if (inode->direct[i] != 0) {
            OOC_FsBlockVisit visit = {
                .event           = OOC_FS_WALK_DATA,
                .logicalIndex    = walk->logicalBase,
                .slot            = &inode->direct[i],
                .containingBlock = 0,
                .containingBuf   = NULL,
            };
            OOC_Error err = walk->visitor(walk, &visit);
            if (err != OOC_ERROR_NONE) {
                return err;
            }
        }
        walk->logicalBase++;
    }
    for (uint32_t level = 0; level < OOC_FS_INDIRECT_LEVELS; level++) {
        walk->slotPtr        = &inode->indirect[level];
        walk->containingBlock = 0;
        walk->containingBuf   = NULL;
        walk->level           = level;
        OOC_Error err = ooc_fsWalkIndirect(walk);
        if (err != OOC_ERROR_NONE) {
            return err;
        }
    }
    return OOC_ERROR_NONE;
}

/* -------------------------------------------------------------------------
 * ooc_fsInodeAllocBlock — find the first free direct-block slot in inode,
 * allocate a data block written with blockBuf, and store the absolute block
 * number in that slot.  The caller is responsible for updating inode.size
 * and writing the inode back.
 * ------------------------------------------------------------------------- */
static OOC_Error ooc_fsInodeAllocBlock(OOC_Fs* fs, OOC_FsInodeDisk* inode, const void* blockBuf) {
    for (uint32_t i = 0; i < OOC_FS_DIRECT_BLOCKS; i++) {
        if (inode->direct[i] == 0) {
            uint32_t blockIdx;
            OOC_Error err = ooc_fsAddDataBlock(fs, blockBuf, &blockIdx);
            if (err != OOC_ERROR_NONE) {
                return err;
            }
            inode->direct[i] = fs->superblock.dataBlock + blockIdx;
            return OOC_ERROR_NONE;
        }
    }
    /* TODO: handle single and double indirect blocks */
    return OOC_ERROR_NO_SPACE;
}

/* -------------------------------------------------------------------------
 * ooc_fsStat
 * ------------------------------------------------------------------------- */

OOC_Error ooc_fsStat(void* self, const OOC_FsFile* file, OOC_FsStat* out) {
    if (!self || !file || !out) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_fsClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Fs* fs = self;
    if (!fs->mounted) {
        return OOC_ERROR_INVALID_STATE;
    }
    OOC_FsInodeDisk inode;
    OOC_Error err = ooc_fsReadInode(fs, file->inodeNum, &inode);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    out->mode      = inode.mode;
    out->uid       = inode.uid;
    out->gid       = inode.gid;
    out->size      = inode.size;
    out->linkCount = inode.linkCount;
    out->atime     = inode.atime;
    out->mtime     = inode.mtime;
    out->ctime     = inode.ctime;
    return OOC_ERROR_NONE;
}

/* -------------------------------------------------------------------------
 * Directory walking: shared visitor that iterates OOC_FsDirentDisk entries
 * in every data block of a directory inode, calling an inner per-entry
 * callback.  Returning OOC_ERROR_STOP from the callback terminates the walk.
 * ------------------------------------------------------------------------- */

typedef OOC_Error (*OOC_FsDirentVisitor)(OOC_Fs* fs, const OOC_FsDirentDisk* entry, void* ctx);

typedef struct {
    OOC_FsDirentVisitor entryVisitor;
    void*               entryCtx;
} OOC_FsDirWalkCtx;

static OOC_Error ooc_fsDirBlockVisitor(OOC_FsWalk* walk, OOC_FsBlockVisit* visit) {
    if (visit->event != OOC_FS_WALK_DATA) {
        return OOC_ERROR_NONE;
    }
    OOC_FsDirWalkCtx* ctx = walk->ctx;
    OOC_Fs* fs = walk->fs;
    uint8_t* buf = malloc(fs->superblock.blockSize);
    if (!buf) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    OOC_Error err = ooc_abstractBlockDeviceRead(fs->device, *visit->slot, buf);
    if (err != OOC_ERROR_NONE) {
        free(buf);
        return err;
    }
    uint32_t entriesPerBlock = fs->superblock.blockSize / (uint32_t)sizeof(OOC_FsDirentDisk);
    OOC_FsDirentDisk* entries = (OOC_FsDirentDisk*)buf;
    for (uint32_t i = 0; i < entriesPerBlock; i++) {
        if (entries[i].inode == 0) {
            continue;
        }
        err = ctx->entryVisitor(fs, &entries[i], ctx->entryCtx);
        if (err != OOC_ERROR_NONE) {
            break;
        }
    }
    free(buf);
    return err;
}

static OOC_Error ooc_fsDirWalk(OOC_Fs* fs, uint32_t dirInodeNum,
                                OOC_FsDirentVisitor entryVisitor, void* entryCtx) {
    OOC_FsInodeDisk inode;
    OOC_Error err = ooc_fsReadInode(fs, dirInodeNum, &inode);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    if (!OOC_FS_IS_DIR(inode.mode)) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_FsDirWalkCtx ctx = { .entryVisitor = entryVisitor, .entryCtx = entryCtx };
    OOC_FsWalk walk = {
        .fs       = fs,
        .inode    = &inode,
        .inodeNum = dirInodeNum,
        .visitor  = ooc_fsDirBlockVisitor,
        .ctx      = &ctx,
    };
    return ooc_fsWalkBlocks(&walk);
}

/* -------------------------------------------------------------------------
 * ooc_fsLookup
 * ------------------------------------------------------------------------- */

typedef struct {
    const char*  name;
    uint8_t      nameLen;
    OOC_FsFile*  result;
} OOC_FsLookupCtx;

static OOC_Error ooc_fsLookupEntry(OOC_Fs* fs, const OOC_FsDirentDisk* entry, void* ctx) {
    (void)fs;
    OOC_FsLookupCtx* lctx = ctx;
    if (entry->nameLen == lctx->nameLen &&
        memcmp(entry->name, lctx->name, lctx->nameLen) == 0) {
        lctx->result = ooc_fsAllocFileHandle(entry->inode);
        if (!lctx->result) {
            return OOC_ERROR_OUT_OF_MEMORY;
        }
        return OOC_ERROR_STOP;
    }
    return OOC_ERROR_NONE;
}

OOC_Error ooc_fsLookup(void* self, const OOC_FsFile* dir, const char* name, OOC_FsFile** out) {
    if (!self || !dir || !name || !out) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_fsClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Fs* fs = self;
    if (!fs->mounted) {
        return OOC_ERROR_INVALID_STATE;
    }
    size_t nameLen = strlen(name);
    if (nameLen == 0 || nameLen > OOC_FS_NAME_MAX) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_FsLookupCtx ctx = { .name = name, .nameLen = (uint8_t)nameLen, .result = NULL };
    OOC_Error err = ooc_fsDirWalk(fs, dir->inodeNum, ooc_fsLookupEntry, &ctx);
    if (err == OOC_ERROR_STOP) {
        *out = ctx.result;
        return OOC_ERROR_NONE;
    }
    if (err == OOC_ERROR_NONE) {
        return OOC_ERROR_NOT_FOUND;
    }
    return err;
}

/* -------------------------------------------------------------------------
 * ooc_fsReadDir
 * ------------------------------------------------------------------------- */

typedef struct {
    uint32_t          index;
    uint32_t          count;
    OOC_FsDirentInfo* out;
} OOC_FsReadDirCtx;

static OOC_Error ooc_fsReadDirEntry(OOC_Fs* fs, const OOC_FsDirentDisk* entry, void* ctx) {
    OOC_FsReadDirCtx* rctx = ctx;
    if (rctx->count == rctx->index) {
        OOC_FsInodeDisk inode;
        OOC_Error err = ooc_fsReadInode(fs, entry->inode, &inode);
        if (err != OOC_ERROR_NONE) {
            return err;
        }
        rctx->out->mode = inode.mode;
        memcpy(rctx->out->name, entry->name, entry->nameLen + 1u);
        rctx->count++;
        return OOC_ERROR_STOP;
    }
    rctx->count++;
    return OOC_ERROR_NONE;
}

OOC_Error ooc_fsReadDir(void* self, const OOC_FsFile* dir, uint32_t index, OOC_FsDirentInfo* out) {
    if (!self || !dir || !out) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_fsClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Fs* fs = self;
    if (!fs->mounted) {
        return OOC_ERROR_INVALID_STATE;
    }
    OOC_FsReadDirCtx ctx = { .index = index, .count = 0, .out = out };
    OOC_Error err = ooc_fsDirWalk(fs, dir->inodeNum, ooc_fsReadDirEntry, &ctx);
    if (err == OOC_ERROR_STOP) {
        return OOC_ERROR_NONE;
    }
    if (err == OOC_ERROR_NONE) {
        return OOC_ERROR_NOT_FOUND;
    }
    return err;
}

/* -------------------------------------------------------------------------
 * ooc_fsDirAddEntry — write a dirent into dir, reusing a free (inode==0)
 * slot in an existing data block or appending a new block.
 * ------------------------------------------------------------------------- */

typedef struct {
    const OOC_FsDirentDisk* entry;
    bool                    written;
} OOC_FsDirFreeSlotCtx;

static OOC_Error ooc_fsDirFreeSlotVisitor(OOC_FsWalk* walk, OOC_FsBlockVisit* visit) {
    if (visit->event != OOC_FS_WALK_DATA) {
        return OOC_ERROR_NONE;
    }
    OOC_FsDirFreeSlotCtx* ctx = walk->ctx;
    OOC_Fs* fs = walk->fs;
    uint32_t blockSize = fs->superblock.blockSize;
    uint8_t* buf = malloc(blockSize);
    if (!buf) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    OOC_Error err = ooc_abstractBlockDeviceRead(fs->device, *visit->slot, buf);
    if (err != OOC_ERROR_NONE) {
        free(buf);
        return err;
    }
    uint32_t entriesPerBlock = blockSize / (uint32_t)sizeof(OOC_FsDirentDisk);
    OOC_FsDirentDisk* entries = (OOC_FsDirentDisk*)buf;
    for (uint32_t i = 0; i < entriesPerBlock; i++) {
        if (entries[i].inode == 0) {
            entries[i] = *ctx->entry;
            err = ooc_abstractBlockDeviceWrite(fs->device, *visit->slot, buf);
            free(buf);
            if (err == OOC_ERROR_NONE) {
                ctx->written = true;
                return OOC_ERROR_STOP;
            }
            return err;
        }
    }
    free(buf);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_fsDirAddEntry(OOC_Fs* fs, uint32_t dirInodeNum,
                                    const char* name, uint32_t childInodeNum) {
    OOC_FsDirentDisk entry;
    memset(&entry, 0, sizeof(entry));
    entry.inode   = childInodeNum;
    entry.nameLen = (uint8_t)strlen(name);
    memcpy(entry.name, name, entry.nameLen + 1u);

    OOC_FsInodeDisk inode;
    OOC_Error err = ooc_fsReadInode(fs, dirInodeNum, &inode);
    if (err != OOC_ERROR_NONE) {
        return err;
    }

    /* Try to write into an existing free slot */
    OOC_FsDirFreeSlotCtx slotCtx = { .entry = &entry, .written = false };
    OOC_FsWalk walk = {
        .fs       = fs,
        .inode    = &inode,
        .inodeNum = dirInodeNum,
        .visitor  = ooc_fsDirFreeSlotVisitor,
        .ctx      = &slotCtx,
    };
    err = ooc_fsWalkBlocks(&walk);
    if (err == OOC_ERROR_STOP && slotCtx.written) {
        return OOC_ERROR_NONE;
    }
    if (err != OOC_ERROR_NONE && err != OOC_ERROR_STOP) {
        return err;
    }

    /* No free slot — append a new block */
    uint8_t* buf = calloc(1, fs->superblock.blockSize);
    if (!buf) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    *(OOC_FsDirentDisk*)buf = entry;
    err = ooc_fsInodeAllocBlock(fs, &inode, buf);
    free(buf);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    inode.size += fs->superblock.blockSize;
    return ooc_fsWriteInode(fs, dirInodeNum, &inode);
}

/* -------------------------------------------------------------------------
 * ooc_fsMkdir
 * ------------------------------------------------------------------------- */

OOC_Error ooc_fsMkdir(void* self, const OOC_FsFile* parent, const char* name, uint32_t mode) {
    uint32_t parentInodeNum = parent ? parent->inodeNum : 0;
    if (!self || !parent || !name) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_fsClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Fs* fs = self;
    if (!fs->mounted) {
        return OOC_ERROR_INVALID_STATE;
    }
    size_t nameLen = strlen(name);
    if (nameLen == 0 || nameLen > OOC_FS_NAME_MAX) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }

    /* Reserve the inode number first so "." can embed it correctly */
    uint32_t newInodeNum;
    OOC_Error err = ooc_fsAllocInode(fs, &newInodeNum);
    if (err != OOC_ERROR_NONE) {
        return err;
    }

    /* Build the initial directory block: "." and ".." */
    uint8_t* buf = calloc(1, fs->superblock.blockSize);
    if (!buf) {
        ooc_fsClearInodeBit(fs, newInodeNum);
        fs->superblock.freeInodes++;
        ooc_fsWriteSuperblock(fs);
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    OOC_FsDirentDisk* entries = (OOC_FsDirentDisk*)buf;
    entries[0].inode = newInodeNum;    entries[0].nameLen = 1; memcpy(entries[0].name, ".",  2);
    entries[1].inode = parentInodeNum; entries[1].nameLen = 2; memcpy(entries[1].name, "..", 3);

    /* Build the new directory inode */
    uint32_t now = (uint32_t)time(NULL);
    OOC_FsInodeDisk inode;
    memset(&inode, 0, sizeof(inode));
    inode.mode      = OOC_FS_MODE_DIR | (mode & 0x1FFu);
    inode.linkCount = 2; /* "." + entry in parent */
    inode.size      = 2u * (uint32_t)sizeof(OOC_FsDirentDisk);
    inode.atime     = now;
    inode.mtime     = now;
    inode.ctime     = now;

    err = ooc_fsInodeAllocBlock(fs, &inode, buf);
    free(buf);
    if (err != OOC_ERROR_NONE) {
        ooc_fsClearInodeBit(fs, newInodeNum);
        fs->superblock.freeInodes++;
        ooc_fsWriteSuperblock(fs);
        return err;
    }

    err = ooc_fsWriteInode(fs, newInodeNum, &inode);
    if (err != OOC_ERROR_NONE) {
        ooc_fsClearInodeBit(fs, newInodeNum);
        fs->superblock.freeInodes++;
        ooc_fsWriteSuperblock(fs);
        return err;
    }

    /* Add entry in the parent directory */
    err = ooc_fsDirAddEntry(fs, parentInodeNum, name, newInodeNum);
    if (err != OOC_ERROR_NONE) {
        return err;
    }

    /* Increment parent's linkCount for the ".." back-reference */
    OOC_FsInodeDisk parentInode;
    err = ooc_fsReadInode(fs, parentInodeNum, &parentInode);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    parentInode.linkCount++;
    parentInode.mtime = now;
    parentInode.ctime = now;
    return ooc_fsWriteInode(fs, parentInodeNum, &parentInode);
}

/* -------------------------------------------------------------------------
 * ooc_fsCreate
 * ------------------------------------------------------------------------- */

OOC_Error ooc_fsCreate(void* self, const OOC_FsFile* parent, const char* name, uint32_t mode) {
    uint32_t parentInodeNum = parent ? parent->inodeNum : 0;
    if (!self || !parent || !name) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_fsClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Fs* fs = self;
    if (!fs->mounted) {
        return OOC_ERROR_INVALID_STATE;
    }
    size_t nameLen = strlen(name);
    if (nameLen == 0 || nameLen > OOC_FS_NAME_MAX) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }

    uint32_t now = (uint32_t)time(NULL);
    OOC_FsInodeDisk inode;
    memset(&inode, 0, sizeof(inode));
    inode.mode      = OOC_FS_MODE_FILE | (mode & 0x1FFu);
    inode.linkCount = 1;
    inode.atime     = now;
    inode.mtime     = now;
    inode.ctime     = now;

    uint32_t newInodeNum;
    OOC_Error err = ooc_fsAddInode(fs, &inode, &newInodeNum);
    if (err != OOC_ERROR_NONE) {
        return err;
    }

    err = ooc_fsDirAddEntry(fs, parentInodeNum, name, newInodeNum);
    if (err != OOC_ERROR_NONE) {
        return err;
    }

    /* Update parent timestamps */
    OOC_FsInodeDisk parentInode;
    err = ooc_fsReadInode(fs, parentInodeNum, &parentInode);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    parentInode.mtime = now;
    parentInode.ctime = now;
    return ooc_fsWriteInode(fs, parentInodeNum, &parentInode);
}

/* -------------------------------------------------------------------------
 * Filesystem layout calculation and formatting
 * ------------------------------------------------------------------------- */

static OOC_Error ooc_fsCalculateLayout(OOC_Fs* fs) {
    size_t blockSize   = ooc_abstractBlockDeviceGetBlockSize(fs->device);
    size_t totalBlocks = ooc_abstractBlockDeviceGetTotalBlocks(fs->device);

    if (blockSize < sizeof(OOC_FsInodeDisk)) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    if (blockSize % sizeof(OOC_FsInodeDisk) != 0) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    if (blockSize % sizeof(OOC_FsDirentDisk) != 0) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }

    uint32_t bitsPerBlock   = (uint32_t)(blockSize * 8);
    uint32_t inodesPerBlock = (uint32_t)(blockSize / sizeof(OOC_FsInodeDisk));

    uint64_t fsSizeInBytes = (uint64_t)totalBlocks * blockSize;
    uint32_t totalInodes   = (uint32_t)(fsSizeInBytes / (4 * 1024));
    if (totalInodes < 16) {
        totalInodes = 16;
    }
    totalInodes = ((totalInodes + bitsPerBlock - 1) / bitsPerBlock) * bitsPerBlock;

    uint32_t inodeBitmapCount = totalInodes / bitsPerBlock;
    uint32_t inodeTableCount  = totalInodes / inodesPerBlock;

    uint32_t fixedMeta = 1 + inodeBitmapCount + inodeTableCount;
    if (fixedMeta >= (uint32_t)totalBlocks) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }

    uint32_t remaining       = (uint32_t)totalBlocks - fixedMeta;
    uint32_t dataBitmapCount = (remaining + bitsPerBlock) / (bitsPerBlock + 1);
    if (dataBitmapCount == 0) {
        dataBitmapCount = 1;
    }
    uint32_t dataCount = remaining - dataBitmapCount;

    if (dataCount < 1) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }

    uint32_t inodeBitmapBlock = 1;
    uint32_t dataBitmapBlock  = inodeBitmapBlock + inodeBitmapCount;
    uint32_t inodeTableBlock  = dataBitmapBlock  + dataBitmapCount;
    uint32_t dataBlock        = inodeTableBlock  + inodeTableCount;

    OOC_FsSuperblockDisk* sb = &fs->superblock;
    memset(sb, 0, sizeof(*sb));
    sb->magic            = OOC_FS_MAGIC;
    sb->blockSize        = (uint32_t)blockSize;
    sb->totalBlocks      = (uint32_t)totalBlocks;
    sb->totalInodes      = totalInodes;
    sb->freeBlocks       = dataCount;
    sb->freeInodes       = totalInodes;
    sb->inodeBitmapBlock = inodeBitmapBlock;
    sb->inodeBitmapCount = inodeBitmapCount;
    sb->dataBitmapBlock  = dataBitmapBlock;
    sb->dataBitmapCount  = dataBitmapCount;
    sb->inodeTableBlock  = inodeTableBlock;
    sb->inodeTableCount  = inodeTableCount;
    sb->dataBlock        = dataBlock;
    sb->dataCount        = dataCount;
    sb->rootInode        = 0;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_fsZeroMetadata(OOC_Fs* fs) {
    uint8_t* buf = calloc(1, fs->superblock.blockSize);
    if (!buf) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    OOC_Error err;
    for (uint32_t i = 0; i < fs->superblock.inodeBitmapCount; i++) {
        err = ooc_abstractBlockDeviceWrite(fs->device, fs->superblock.inodeBitmapBlock + i, buf);
        if (err != OOC_ERROR_NONE) {
            free(buf);
            return err;
        }
    }
    for (uint32_t i = 0; i < fs->superblock.dataBitmapCount; i++) {
        err = ooc_abstractBlockDeviceWrite(fs->device, fs->superblock.dataBitmapBlock + i, buf);
        if (err != OOC_ERROR_NONE) {
            free(buf);
            return err;
        }
    }
    for (uint32_t i = 0; i < fs->superblock.inodeTableCount; i++) {
        err = ooc_abstractBlockDeviceWrite(fs->device, fs->superblock.inodeTableBlock + i, buf);
        if (err != OOC_ERROR_NONE) {
            free(buf);
            return err;
        }
    }
    free(buf);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_fsInitRootDir(OOC_Fs* fs) {
    uint8_t* buf = calloc(1, fs->superblock.blockSize);
    if (!buf) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    OOC_FsDirentDisk* entries = (OOC_FsDirentDisk*)buf;
    entries[0].inode = 0; entries[0].nameLen = 1; memcpy(entries[0].name, ".",  2);
    entries[1].inode = 0; entries[1].nameLen = 2; memcpy(entries[1].name, "..", 3);
    uint32_t blockIndex;
    OOC_Error err = ooc_fsAddDataBlock(fs, buf, &blockIndex);
    free(buf);
    if (err != OOC_ERROR_NONE) {
        return err;
    }

    OOC_FsInodeDisk rootInode;
    memset(&rootInode, 0, sizeof(rootInode));
    rootInode.mode      = OOC_FS_MODE_DIR
                        | OOC_FS_PERM_OWNER_R | OOC_FS_PERM_OWNER_W | OOC_FS_PERM_OWNER_X
                        | OOC_FS_PERM_GROUP_R | OOC_FS_PERM_GROUP_X
                        | OOC_FS_PERM_OTHER_R | OOC_FS_PERM_OTHER_X;
    rootInode.linkCount = 2;
    rootInode.size      = 2 * (uint32_t)sizeof(OOC_FsDirentDisk);
    rootInode.direct[0] = fs->superblock.dataBlock + blockIndex;
    uint32_t rootInodeNum;
    err = ooc_fsAddInode(fs, &rootInode, &rootInodeNum);
    if (err != OOC_ERROR_NONE) {
        return err;
    }

    fs->superblock.rootInode = rootInodeNum;
    return ooc_fsWriteSuperblock(fs);
}

OOC_Error ooc_fsFormat(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_fsClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Fs* fs = self;
    OOC_Error err = ooc_fsCalculateLayout(fs);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    err = ooc_fsWriteSuperblock(fs);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    err = ooc_fsZeroMetadata(fs);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    return ooc_fsInitRootDir(fs);
}

OOC_Error ooc_fsMount(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_fsClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Fs* fs = self;
    if (fs->mounted) {
        return OOC_ERROR_INVALID_STATE;
    }
    uint8_t* buf = malloc(ooc_abstractBlockDeviceGetBlockSize(fs->device));
    if (!buf) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    OOC_Error err = ooc_abstractBlockDeviceRead(fs->device, 0, buf);
    if (err != OOC_ERROR_NONE) {
        free(buf);
        return err;
    }
    OOC_FsSuperblockDisk* sb = (OOC_FsSuperblockDisk*)buf;
    if (sb->magic != OOC_FS_MAGIC) {
        free(buf);
        return OOC_ERROR_INVALID_FORMAT;
    }
    if (sb->blockSize != ooc_abstractBlockDeviceGetBlockSize(fs->device)) {
        free(buf);
        return OOC_ERROR_INVALID_FORMAT;
    }
    if (sb->totalBlocks != ooc_abstractBlockDeviceGetTotalBlocks(fs->device)) {
        free(buf);
        return OOC_ERROR_INVALID_FORMAT;
    }
    memcpy(&fs->superblock, sb, sizeof(fs->superblock));
    free(buf);
    fs->mounted = true;
    return OOC_ERROR_NONE;
}

OOC_Error ooc_fsUnmount(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_fsClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Fs* fs = self;
    if (!fs->mounted) {
        return OOC_ERROR_INVALID_STATE;
    }
    OOC_Error err = ooc_fsWriteSuperblock(fs);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    fs->mounted = false;
    return OOC_ERROR_NONE;
}

OOC_Error ooc_fsGetRoot(void* self, OOC_FsFile** out) {
    if (!self || !out) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_fsClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Fs* fs = self;
    if (!fs->mounted) {
        return OOC_ERROR_INVALID_STATE;
    }
    *out = ooc_fsAllocFileHandle(fs->superblock.rootInode);
    if (!*out) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_fsCtor(void* self, va_list* args) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_fsClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error err = ooc_superCtor(ooc_fsClass(), self, args);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    OOC_Fs* fs = self;
    memset((char*)fs + offsetof(OOC_Fs, device), 0, sizeof(*fs) - offsetof(OOC_Fs, device));
    fs->device  = va_arg(*args, void*);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_fsDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_fsClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Fs* fs = self;
    memset((char*)fs + offsetof(OOC_Fs, device), 0, sizeof(*fs) - offsetof(OOC_Fs, device));
    return ooc_superDtor(ooc_fsClass(), self);
}

static void* ooc_fsClassInit(void) {
    if (ooc_classNew(&FsClassInstance,
                    "Fs",
                    sizeof(OOC_Fs),
                    sizeof(OOC_FsClass),
                    ooc_objectClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_fsCtor,
                    OOC_METHOD_DTOR, ooc_fsDtor,
                    0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&FsClassInstance);
        return NULL;
    }
    return &FsClassInstance;
}

void* ooc_fsClass(void) {
    if (!FsClass) {
        FsClass = ooc_fsClassInit();
    }

    return FsClass;
}
