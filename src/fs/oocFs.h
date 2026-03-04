#ifndef OOC_FS_H_
#define OOC_FS_H_

#include "oocError.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum file/directory name length (excluding null terminator) */
#define OOC_FS_NAME_MAX 55

/* File type bits (stored in upper 16 bits of mode) */
#define OOC_FS_MODE_FILE    0x00010000
#define OOC_FS_MODE_DIR     0x00020000
#define OOC_FS_MODE_SYMLINK 0x00040000

/* Permission bits (lower 9 bits, POSIX-style) */
#define OOC_FS_PERM_OWNER_R 0400
#define OOC_FS_PERM_OWNER_W 0200
#define OOC_FS_PERM_OWNER_X 0100
#define OOC_FS_PERM_GROUP_R 0040
#define OOC_FS_PERM_GROUP_W 0020
#define OOC_FS_PERM_GROUP_X 0010
#define OOC_FS_PERM_OTHER_R 0004
#define OOC_FS_PERM_OTHER_W 0002
#define OOC_FS_PERM_OTHER_X 0001

/* Convenience macros to test the type bits of a mode value */
#define OOC_FS_IS_FILE(mode)    (((mode) & OOC_FS_MODE_FILE)    != 0)
#define OOC_FS_IS_DIR(mode)     (((mode) & OOC_FS_MODE_DIR)     != 0)
#define OOC_FS_IS_SYMLINK(mode) (((mode) & OOC_FS_MODE_SYMLINK) != 0)

/*
 * Opaque file/directory handle.
 * Obtained via ooc_fsGetRoot or ooc_fsLookup.
 * Must be released with ooc_fsFileClose when no longer needed.
 */
typedef struct OOC_FsFile OOC_FsFile;

/*
 * File metadata returned by ooc_fsStat().
 */
typedef struct OOC_FsStat {
    uint32_t mode;
    uint32_t uid;
    uint32_t gid;
    uint32_t size;
    uint32_t linkCount;
    uint32_t atime;
    uint32_t mtime;
    uint32_t ctime;
} OOC_FsStat;

/*
 * Directory entry returned by ooc_fsReadDir().
 * Use ooc_fsLookup with entry.name to obtain a handle for the entry.
 */
typedef struct OOC_FsDirentInfo {
    uint32_t mode;
    char     name[OOC_FS_NAME_MAX + 1];
} OOC_FsDirentInfo;

void* ooc_fsClass(void);

/*
 * Formats a block device with a fresh OOC filesystem.
 * The device must be an AbstractBlockDevice with a block size that is a
 * multiple of 128 bytes (the inode size).
 * Any existing data on the device is overwritten.
 */
OOC_Error ooc_fsFormat(void* self);

/*
 * Mounts a formatted filesystem.
 * Reads and validates the superblock from the device.
 * Must be called before any filesystem operations.
 */
OOC_Error ooc_fsMount(void* self);

/*
 * Unmounts the filesystem.
 * Flushes the superblock back to disk.
 */
OOC_Error ooc_fsUnmount(void* self);

/*
 * Releases a file handle obtained from ooc_fsGetRoot or ooc_fsLookup.
 */
void ooc_fsFileClose(OOC_FsFile* file);

/*
 * Returns a handle to the root directory.
 * Caller must close it with ooc_fsFileClose.
 */
OOC_Error ooc_fsGetRoot(void* self, OOC_FsFile** out);

/*
 * Fills *out with metadata for the file or directory referred to by file.
 */
OOC_Error ooc_fsStat(void* self, const OOC_FsFile* file, OOC_FsStat* out);

/*
 * Searches directory dir for an entry named name.
 * On success *out receives a new handle. Caller must close it.
 * Returns OOC_ERROR_NOT_FOUND if no matching entry exists.
 */
OOC_Error ooc_fsLookup(void* self, const OOC_FsFile* dir, const char* name, OOC_FsFile** out);

/*
 * Returns the index-th non-deleted entry in directory dir (0-based).
 * Fills *out with the entry's mode and name.
 * Use ooc_fsLookup with entry.name to obtain a handle for the entry.
 * Returns OOC_ERROR_NOT_FOUND when index is past the last entry.
 */
OOC_Error ooc_fsReadDir(void* self, const OOC_FsFile* dir, uint32_t index, OOC_FsDirentInfo* out);

/*
 * Creates a new subdirectory named name inside parent.
 * mode supplies the permission bits (lower 9 bits); OOC_FS_MODE_DIR is
 * added automatically.
 */
OOC_Error ooc_fsMkdir(void* self, const OOC_FsFile* parent, const char* name, uint32_t mode);

/*
 * Creates a new regular file named name inside parent.
 * mode supplies the permission bits (lower 9 bits); OOC_FS_MODE_FILE is
 * added automatically.
 */
OOC_Error ooc_fsCreate(void* self, const OOC_FsFile* parent, const char* name, uint32_t mode);

#endif
