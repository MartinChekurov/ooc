#ifndef OOC_MEMORY_BLOCK_DEVICE_H_
#define OOC_MEMORY_BLOCK_DEVICE_H_

#include "oocError.h"
#include <stddef.h>

/*
 * Concrete block device that stores data in a malloc'd memory buffer.
 *
 * Constructor parameters:
 *   totalBlocks (size_t): number of blocks
 *   blockSize   (size_t): size of each block in bytes
 *
 * Example:
 *   void* dev = ooc_new(ooc_memoryBlockDeviceClass(), (size_t)128, (size_t)512);
 */
void* ooc_memoryBlockDeviceClass(void);

OOC_Error ooc_memoryBlockDeviceWrite(const void* self, size_t block, const void* buffer);
OOC_Error ooc_memoryBlockDeviceRead(const void* self, size_t block, void* buffer);
size_t ooc_memoryBlockDeviceGetBlockSize(const void* self);
size_t ooc_memoryBlockDeviceGetTotalBlocks(const void* self);

#endif
