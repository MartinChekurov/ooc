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

static OOC_Error ooc_fsAddInode(OOC_Fs* fs, const OOC_FsInodeDisk* inode, uint32_t* outInodeNum) {
    OOC_Error err = ooc_fsGetFreeInode(fs, outInodeNum);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    err = ooc_fsSetInodeBit(fs, *outInodeNum);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    fs->superblock.freeInodes--;
    err = ooc_fsWriteSuperblock(fs);
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

static OOC_Error ooc_fsResolveBlockPtr(OOC_Fs* fs, const OOC_FsInodeDisk* inode,
                                        uint32_t logicalIndex, uint32_t* outAbsBlock) {
    uint32_t ptrsPerBlock = ooc_fsGetPtrsPerBlock(fs);

    if (logicalIndex < OOC_FS_DIRECT_BLOCKS) {
        *outAbsBlock = inode->direct[logicalIndex];
        return OOC_ERROR_NONE;
    }
    logicalIndex -= OOC_FS_DIRECT_BLOCKS;

    uint32_t* buf = malloc(fs->superblock.blockSize);
    if (!buf) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }

    uint64_t rangeSize = ptrsPerBlock;
    for (uint32_t level = 0; level < OOC_FS_INDIRECT_LEVELS; level++) {
        if ((uint64_t)logicalIndex < rangeSize) {
            /* Decompose logicalIndex: indices[0]=innermost, indices[level]=outermost */
            uint32_t indices[OOC_FS_INDIRECT_LEVELS];
            uint32_t tmp = logicalIndex;
            for (uint32_t d = 0; d <= level; d++) {
                indices[d] = tmp % ptrsPerBlock;
                tmp /= ptrsPerBlock;
            }
            /* Walk from outermost to innermost, reusing buf */
            uint32_t block = inode->indirect[level];
            for (int d = (int)level; d >= 0; d--) {
                if (block == 0) {
                    free(buf);
                    *outAbsBlock = 0;
                    return OOC_ERROR_NONE;
                }
                OOC_Error err = ooc_abstractBlockDeviceRead(fs->device, block, buf);
                if (err != OOC_ERROR_NONE) {
                    free(buf);
                    return err;
                }
                block = buf[indices[d]];
            }
            free(buf);
            *outAbsBlock = block;
            return OOC_ERROR_NONE;
        }
        logicalIndex -= (uint32_t)rangeSize;
        rangeSize *= ptrsPerBlock;
    }

    free(buf);
    return OOC_ERROR_OUT_OF_BOUNDS;
}

static OOC_Error ooc_fsSetBlockPtr(OOC_Fs* fs, OOC_FsInodeDisk* inode,
                                   uint32_t logicalIndex, uint32_t absBlock) {
    uint32_t ptrsPerBlock = ooc_fsGetPtrsPerBlock(fs);

    if (logicalIndex < OOC_FS_DIRECT_BLOCKS) {
        inode->direct[logicalIndex] = absBlock;
        return OOC_ERROR_NONE;
    }
    logicalIndex -= OOC_FS_DIRECT_BLOCKS;

    uint32_t* buf = malloc(fs->superblock.blockSize);
    if (!buf) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }

    uint64_t rangeSize = ptrsPerBlock;
    for (uint32_t level = 0; level < OOC_FS_INDIRECT_LEVELS; level++) {
        if ((uint64_t)logicalIndex < rangeSize) {
            uint32_t indices[OOC_FS_INDIRECT_LEVELS];
            uint32_t tmp = logicalIndex;
            for (uint32_t d = 0; d <= level; d++) {
                indices[d] = tmp % ptrsPerBlock;
                tmp /= ptrsPerBlock;
            }
            /* Allocate root table block if needed */
            if (inode->indirect[level] == 0) {
                uint8_t* zero = calloc(1, fs->superblock.blockSize);
                if (!zero) {
                    free(buf);
                    return OOC_ERROR_OUT_OF_MEMORY;
                }
                uint32_t relIdx;
                OOC_Error err = ooc_fsAddDataBlock(fs, zero, &relIdx);
                free(zero);
                if (err != OOC_ERROR_NONE) {
                    free(buf);
                    return err;
                }
                inode->indirect[level] = fs->superblock.dataBlock + relIdx;
            }
            /* Walk from outermost to innermost, allocating table blocks on demand */
            uint32_t block = inode->indirect[level];
            for (int d = (int)level; d >= 0; d--) {
                OOC_Error err = ooc_abstractBlockDeviceRead(fs->device, block, buf);
                if (err != OOC_ERROR_NONE) {
                    free(buf);
                    return err;
                }
                if (d == 0) {
                    buf[indices[0]] = absBlock;
                    err = ooc_abstractBlockDeviceWrite(fs->device, block, buf);
                    free(buf);
                    return err;
                }
                if (buf[indices[d]] == 0) {
                    uint8_t* zero = calloc(1, fs->superblock.blockSize);
                    if (!zero) {
                        free(buf);
                        return OOC_ERROR_OUT_OF_MEMORY;
                    }
                    uint32_t relIdx;
                    err = ooc_fsAddDataBlock(fs, zero, &relIdx);
                    free(zero);
                    if (err != OOC_ERROR_NONE) {
                        free(buf);
                        return err;
                    }
                    buf[indices[d]] = fs->superblock.dataBlock + relIdx;
                    err = ooc_abstractBlockDeviceWrite(fs->device, block, buf);
                    if (err != OOC_ERROR_NONE) {
                        free(buf);
                        return err;
                    }
                }
                block = buf[indices[d]];
            }
            free(buf);
            return OOC_ERROR_NONE;
        }
        logicalIndex -= (uint32_t)rangeSize;
        rangeSize *= ptrsPerBlock;
    }

    free(buf);
    return OOC_ERROR_OUT_OF_BOUNDS;
}

/*
 * Frees all data blocks at logical indices >= keepBlocks.
 * Updates fs->superblock.freeBlocks in memory; does NOT write superblock.
 * Updates inode block-pointer fields in memory; does NOT write the inode.
 */
static OOC_Error ooc_fsFreeTailBlocks(OOC_Fs* fs, OOC_FsInodeDisk* inode, uint32_t keepBlocks) {
    uint32_t ptrsPerBlock = ooc_fsGetPtrsPerBlock(fs);
    OOC_Error err;

    for (uint32_t i = keepBlocks; i < OOC_FS_DIRECT_BLOCKS; i++) {
        if (inode->direct[i] != 0) {
            uint32_t relIdx = inode->direct[i] - fs->superblock.dataBlock;
            err = ooc_fsClearDataBit(fs, relIdx);
            if (err != OOC_ERROR_NONE) {
                return err;
            }
            fs->superblock.freeBlocks++;
            inode->direct[i] = 0;
        }
    }

    if (inode->indirect[0] != 0) {
        uint32_t indStart = (keepBlocks > OOC_FS_DIRECT_BLOCKS)
                          ? keepBlocks - OOC_FS_DIRECT_BLOCKS : 0;
        if (indStart < ptrsPerBlock) {
            uint32_t* indBuf = malloc(fs->superblock.blockSize);
            if (!indBuf) {
                return OOC_ERROR_OUT_OF_MEMORY;
            }
            err = ooc_abstractBlockDeviceRead(fs->device, inode->indirect[0], indBuf);
            if (err != OOC_ERROR_NONE) {
                free(indBuf);
                return err;
            }
            bool dirty = false;
            for (uint32_t j = indStart; j < ptrsPerBlock; j++) {
                if (indBuf[j] != 0) {
                    uint32_t relIdx = indBuf[j] - fs->superblock.dataBlock;
                    err = ooc_fsClearDataBit(fs, relIdx);
                    if (err != OOC_ERROR_NONE) {
                        free(indBuf);
                        return err;
                    }
                    fs->superblock.freeBlocks++;
                    indBuf[j] = 0;
                    dirty = true;
                }
            }
            if (indStart == 0) {
                free(indBuf);
                uint32_t relIdx = inode->indirect - fs->superblock.dataBlock;
                err = ooc_fsClearDataBit(fs, relIdx);
                if (err != OOC_ERROR_NONE) {
                    return err;
                }
                fs->superblock.freeBlocks++;
                inode->indirect[0] = 0;
            } else if (dirty) {
                err = ooc_abstractBlockDeviceWrite(fs->device, inode->indirect[0], indBuf);
                free(indBuf);
                if (err != OOC_ERROR_NONE) {
                    return err;
                }
            } else {
                free(indBuf);
            }
        }
    }

    if (inode->indirect[1] != 0) {
        uint32_t dblBase  = OOC_FS_DIRECT_BLOCKS + ptrsPerBlock;
        uint32_t dblStart = (keepBlocks > dblBase) ? keepBlocks - dblBase : 0;

        if ((uint64_t)dblStart < (uint64_t)ptrsPerBlock * ptrsPerBlock) {
            uint32_t outerStart = dblStart / ptrsPerBlock;

            uint32_t* outerBuf = malloc(fs->superblock.blockSize);
            if (!outerBuf) {
                return OOC_ERROR_OUT_OF_MEMORY;
            }
            err = ooc_abstractBlockDeviceRead(fs->device, inode->indirect[1], outerBuf);
            if (err != OOC_ERROR_NONE) {
                free(outerBuf);
                return err;
            }

            bool outerDirty = false;
            for (uint32_t outer = outerStart; outer < ptrsPerBlock; outer++) {
                if (outerBuf[outer] == 0) {
                    continue;
                }
                uint32_t innerStart = (outer == outerStart && dblStart % ptrsPerBlock != 0)
                                    ? dblStart % ptrsPerBlock : 0;

                uint32_t* innerBuf = malloc(fs->superblock.blockSize);
                if (!innerBuf) {
                    free(outerBuf);
                    return OOC_ERROR_OUT_OF_MEMORY;
                }
                err = ooc_abstractBlockDeviceRead(fs->device, outerBuf[outer], innerBuf);
                if (err != OOC_ERROR_NONE) {
                    free(innerBuf);
                    free(outerBuf);
                    return err;
                }
                bool innerDirty = false;
                for (uint32_t inner = innerStart; inner < ptrsPerBlock; inner++) {
                    if (innerBuf[inner] != 0) {
                        uint32_t relIdx = innerBuf[inner] - fs->superblock.dataBlock;
                        err = ooc_fsClearDataBit(fs, relIdx);
                        if (err != OOC_ERROR_NONE) {
                            free(innerBuf);
                            free(outerBuf);
                            return err;
                        }
                        fs->superblock.freeBlocks++;
                        innerBuf[inner] = 0;
                        innerDirty = true;
                    }
                }
                if (innerStart == 0) {
                    free(innerBuf);
                    uint32_t relIdx = outerBuf[outer] - fs->superblock.dataBlock;
                    err = ooc_fsClearDataBit(fs, relIdx);
                    if (err != OOC_ERROR_NONE) {
                        free(outerBuf);
                        return err;
                    }
                    fs->superblock.freeBlocks++;
                    outerBuf[outer] = 0;
                    outerDirty = true;
                } else if (innerDirty) {
                    err = ooc_abstractBlockDeviceWrite(fs->device, outerBuf[outer], innerBuf);
                    free(innerBuf);
                    if (err != OOC_ERROR_NONE) {
                        free(outerBuf);
                        return err;
                    }
                } else {
                    free(innerBuf);
                }
            }

            if (dblStart == 0) {
                free(outerBuf);
                uint32_t relIdx = inode->doubleIndirect - fs->superblock.dataBlock;
                err = ooc_fsClearDataBit(fs, relIdx);
                if (err != OOC_ERROR_NONE) {
                    return err;
                }
                fs->superblock.freeBlocks++;
                inode->indirect[1] = 0;
            } else if (outerDirty) {
                err = ooc_abstractBlockDeviceWrite(fs->device, inode->indirect[1], outerBuf);
                free(outerBuf);
                if (err != OOC_ERROR_NONE) {
                    return err;
                }
            } else {
                free(outerBuf);
            }
        }
    }

    return OOC_ERROR_NONE;
}

static uint32_t ooc_fsInodeGetBlockCount(OOC_Fs* fs, const OOC_FsInodeDisk* inode) {
    if (inode->size == 0) {
        return 0;
    }
    return (inode->size + fs->superblock.blockSize - 1) / fs->superblock.blockSize;
}

static OOC_Error ooc_fsInodeReadBlock(OOC_Fs* fs, const OOC_FsInodeDisk* inode,
                                       uint32_t logicalIndex, void* buf) {
    uint32_t absBlock;
    OOC_Error err = ooc_fsResolveBlockPtr(fs, inode, logicalIndex, &absBlock);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    if (absBlock == 0) {
        memset(buf, 0, fs->superblock.blockSize);
        return OOC_ERROR_NONE;
    }
    return ooc_abstractBlockDeviceRead(fs->device, absBlock, buf);
}

static OOC_Error ooc_fsInodeWriteBlock(OOC_Fs* fs, const OOC_FsInodeDisk* inode,
                                        uint32_t logicalIndex, const void* buf) {
    uint32_t absBlock;
    OOC_Error err = ooc_fsResolveBlockPtr(fs, inode, logicalIndex, &absBlock);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    if (absBlock == 0) {
        return OOC_ERROR_NOT_FOUND;
    }
    return ooc_abstractBlockDeviceWrite(fs->device, absBlock, buf);
}

static OOC_Error ooc_fsInodeAppendBlock(OOC_Fs* fs, OOC_FsInodeDisk* inode,
                                         uint32_t inodeNum, const void* buf) {
    uint32_t logicalIndex = ooc_fsInodeGetBlockCount(fs, inode);
    uint32_t relIdx;
    OOC_Error err = ooc_fsAddDataBlock(fs, buf, &relIdx);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    uint32_t absBlock = fs->superblock.dataBlock + relIdx;
    err = ooc_fsSetBlockPtr(fs, inode, logicalIndex, absBlock);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    inode->size = (logicalIndex + 1) * fs->superblock.blockSize;
    return ooc_fsWriteInode(fs, inodeNum, inode);
}

static OOC_Error ooc_fsInodePopBlock(OOC_Fs* fs, OOC_FsInodeDisk* inode, uint32_t inodeNum) {
    uint32_t count = ooc_fsInodeGetBlockCount(fs, inode);
    if (count == 0) {
        return OOC_ERROR_NOT_FOUND;
    }
    OOC_Error err = ooc_fsFreeTailBlocks(fs, inode, count - 1);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    inode->size = (count - 1) * fs->superblock.blockSize;
    err = ooc_fsWriteInode(fs, inodeNum, inode);
    if (err != OOC_ERROR_NONE) {
        return err;
    }
    return ooc_fsWriteSuperblock(fs);
}

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
