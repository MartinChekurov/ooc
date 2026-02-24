#ifndef OOC_FS_R_
#define OOC_FS_R_

#include "oocObject.r"
#include "oocFs.h"
#include <stdint.h>
#include <stdbool.h>

/* Filesystem magic number ("OOFC") */
#define OOC_FS_MAGIC 0x4F4F4643

/* Number of direct block pointers in an inode */
#define OOC_FS_DIRECT_BLOCKS 12

/* Number of indirect levels (1=single, 2=double, 3=triple, ...) */
#define OOC_FS_INDIRECT_LEVELS 2

/*
 * On-disk superblock — stored in block 0.
 * The rest of block 0 beyond sizeof(OOC_FsSuperblockDisk) is unused.
 *
 * Block layout:
 *   [ 0: superblock | inodeBitmapBlock: inode bitmap | dataBitmapBlock: data bitmap
 *     | inodeTableBlock: inode table | dataBlock: data blocks ... ]
 */
typedef struct OOC_FsSuperblockDisk {
    uint32_t magic;             /* OOC_FS_MAGIC */
    uint32_t blockSize;         /* block size in bytes */
    uint32_t totalBlocks;       /* total blocks on the device */
    uint32_t totalInodes;       /* total number of inodes */
    uint32_t freeBlocks;        /* number of free data blocks */
    uint32_t freeInodes;        /* number of free inodes */
    uint32_t inodeBitmapBlock;  /* first block of inode bitmap */
    uint32_t inodeBitmapCount;  /* number of blocks in inode bitmap */
    uint32_t dataBitmapBlock;   /* first block of data bitmap */
    uint32_t dataBitmapCount;   /* number of blocks in data bitmap */
    uint32_t inodeTableBlock;   /* first block of inode table */
    uint32_t inodeTableCount;   /* number of blocks in inode table */
    uint32_t dataBlock;         /* first block of data region */
    uint32_t dataCount;         /* number of data blocks */
    uint32_t rootInode;         /* inode number of root directory */
} OOC_FsSuperblockDisk;        /* 60 bytes */

/*
 * On-disk inode — 128 bytes, so 4 inodes fit in a 512-byte block.
 *
 * Block pointers store absolute block numbers on the device.
 * A value of 0 means the pointer is unused (block 0 is the superblock,
 * never a data block).
 */
typedef struct OOC_FsInodeDisk {
    uint32_t mode;                          /* type + permissions */
    uint32_t uid;
    uint32_t gid;
    uint32_t size;                          /* file size in bytes */
    uint32_t linkCount;
    uint32_t atime;                         /* last access time */
    uint32_t mtime;                         /* last modification time */
    uint32_t ctime;                         /* last status change time */
    uint32_t direct[OOC_FS_DIRECT_BLOCKS];      /* direct block pointers */
    uint32_t indirect[OOC_FS_INDIRECT_LEVELS];  /* indirect[0]=single, [1]=double, ... */
    uint32_t reserved[12 - OOC_FS_INDIRECT_LEVELS]; /* padding to 128 bytes */
} OOC_FsInodeDisk;                         /* 128 bytes */

/*
 * On-disk directory entry — 64 bytes, so 8 entries fit in a 512-byte block.
 * An entry with inode == 0 is free/deleted.
 */
typedef struct OOC_FsDirentDisk {
    uint32_t inode;                     /* inode number*/
    uint8_t  nameLen;                   /* length of name, not including null */
    uint8_t  reserved[3];               /* padding */
    char     name[OOC_FS_NAME_MAX + 1]; /* null-terminated name (56 bytes) */
} OOC_FsDirentDisk;                     /* 64 bytes */

/*
 * Opaque file handle returned by ooc_fsFileCreate and directory lookups.
 * The caller must not inspect or modify its contents directly.
 */
typedef struct OOC_FsFile {
    uint32_t inodeNum;
} OOC_FsFile;

/*
 * In-memory OOC_Fs object.
 * Holds the mounted device and a cached copy of the superblock.
 */
typedef struct OOC_Fs OOC_Fs;
typedef struct OOC_FsClass OOC_FsClass;

struct OOC_Fs {
    OOC_Object object;
    void*               device;     /* AbstractBlockDevice */
    OOC_FsSuperblockDisk superblock; /* cached superblock (written back on unmount) */
    bool                mounted;
};

struct OOC_FsClass {
    OOC_Class class;
};

#endif
