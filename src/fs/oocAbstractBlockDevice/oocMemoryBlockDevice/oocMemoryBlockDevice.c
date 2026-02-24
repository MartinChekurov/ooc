#include "oocMemoryBlockDevice.h"
#include "oocMemoryBlockDevice.r"
#include "oocAbstractBlockDevice.h"
#include "oocAbstractBlockDevice.r"
#include "oocError.h"
#include "oocModifiers.h"
#include "oocObject.h"
#include "oocObject.r"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static OOC_MemoryBlockDeviceClass* MemoryBlockDeviceClass;
static OOC_MemoryBlockDeviceClass MemoryBlockDeviceInstance;

static OOC_Error ooc_memoryBlockDeviceWriteImpl(const void* self, size_t block, const void* buffer) {
    const OOC_AbstractBlockDevice* base = self;
    const OOC_MemoryBlockDevice* device = self;
    memcpy(device->memory + block * base->blockSize, buffer, base->blockSize);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_memoryBlockDeviceReadImpl(const void* self, size_t block, void* buffer) {
    const OOC_AbstractBlockDevice* base = self;
    const OOC_MemoryBlockDevice* device = self;
    memcpy(buffer, device->memory + block * base->blockSize, base->blockSize);
    return OOC_ERROR_NONE;
}

OOC_Error ooc_memoryBlockDeviceWrite(const void* self, size_t block, const void* buffer) {
    return ooc_abstractBlockDeviceWrite(self, block, buffer);
}

OOC_Error ooc_memoryBlockDeviceRead(const void* self, size_t block, void* buffer) {
    return ooc_abstractBlockDeviceRead(self, block, buffer);
}

size_t ooc_memoryBlockDeviceGetBlockSize(const void* self) {
    return ooc_abstractBlockDeviceGetBlockSize(self);
}

size_t ooc_memoryBlockDeviceGetTotalBlocks(const void* self) {
    return ooc_abstractBlockDeviceGetTotalBlocks(self);
}

static OOC_Error ooc_memoryBlockDeviceCtor(void* self, va_list* args) {
    OOC_TYPE_CHECK(self, ooc_memoryBlockDeviceClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(ooc_memoryBlockDeviceClass(), self, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    const OOC_AbstractBlockDevice* base = self;
    OOC_MemoryBlockDevice* device = self;
    device->memory = calloc(base->totalBlocks, base->blockSize);
    if (!device->memory) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_memoryBlockDeviceDtor(void* self) {
    OOC_TYPE_CHECK(self, ooc_memoryBlockDeviceClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_MemoryBlockDevice* device = self;
    free(device->memory);
    device->memory = NULL;
    return ooc_superDtor(ooc_memoryBlockDeviceClass(), self);
}

static void* ooc_memoryBlockDeviceClassInit(void) {
    if (ooc_classNew(&MemoryBlockDeviceInstance,
                    "MemoryBlockDevice",
                    sizeof(OOC_MemoryBlockDevice),
                    sizeof(OOC_MemoryBlockDeviceClass),
                    ooc_abstractBlockDeviceClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_memoryBlockDeviceCtor,
                    OOC_METHOD_DTOR, ooc_memoryBlockDeviceDtor,
                    OOC_ABSTRACT_BLOCK_DEVICE_METHOD_WRITE, ooc_memoryBlockDeviceWriteImpl,
                    OOC_ABSTRACT_BLOCK_DEVICE_METHOD_READ, ooc_memoryBlockDeviceReadImpl,
                    0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&MemoryBlockDeviceInstance);
        return NULL;
    }
    return &MemoryBlockDeviceInstance;
}

void* ooc_memoryBlockDeviceClass(void) {
    if (!MemoryBlockDeviceClass) {
        MemoryBlockDeviceClass = ooc_memoryBlockDeviceClassInit();
    }
    return MemoryBlockDeviceClass;
}
