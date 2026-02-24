#ifndef OOC_MEMORY_BLOCK_DEVICE_R_
#define OOC_MEMORY_BLOCK_DEVICE_R_

#include "oocAbstractBlockDevice.r"

typedef struct OOC_MemoryBlockDevice OOC_MemoryBlockDevice;
typedef struct OOC_MemoryBlockDeviceClass OOC_MemoryBlockDeviceClass;

struct OOC_MemoryBlockDevice {
    OOC_AbstractBlockDevice device;
    unsigned char* memory;
};

struct OOC_MemoryBlockDeviceClass {
    OOC_AbstractBlockDeviceClass class;
};

#endif
