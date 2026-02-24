#ifndef OOC_ABSTRACT_BLOCK_DEVICE_R_
#define OOC_ABSTRACT_BLOCK_DEVICE_R_

#include "oocObject.r"
#include "oocError.h"
#include <stddef.h>

typedef struct OOC_AbstractBlockDevice OOC_AbstractBlockDevice;
typedef struct OOC_AbstractBlockDeviceClass OOC_AbstractBlockDeviceClass;

struct OOC_AbstractBlockDevice {
    OOC_Object object;
    size_t totalBlocks;
    size_t blockSize;
};

struct OOC_AbstractBlockDeviceClass {
    OOC_Class class;
    OOC_Error (*write)(const void* self, size_t block, const void* buffer);
    OOC_Error (*read)(const void* self, size_t block, void* buffer);

};

#define OOC_ABSTRACT_BLOCK_DEVICE_METHOD_WRITE\
 offsetof(OOC_AbstractBlockDeviceClass, write)
#define OOC_ABSTRACT_BLOCK_DEVICE_METHOD_READ\
 offsetof(OOC_AbstractBlockDeviceClass, read)

#endif
