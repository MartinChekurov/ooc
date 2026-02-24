#ifndef OOC_ABSTRACT_BLOCK_DEVICE_H_
#define OOC_ABSTRACT_BLOCK_DEVICE_H_

#include "oocError.h"
#include <stddef.h>

void* ooc_abstractBlockDeviceClass(void);

OOC_Error ooc_abstractBlockDeviceWrite(const void* self, size_t block, const void* buffer);
OOC_Error ooc_abstractBlockDeviceRead(const void* self, size_t block, void* buffer);
size_t ooc_abstractBlockDeviceGetBlockSize(const void* self);
size_t ooc_abstractBlockDeviceGetTotalBlocks(const void* self);

#endif
