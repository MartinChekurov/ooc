#include "oocAbstractBlockDevice.h"
#include "oocAbstractBlockDevice.r"
#include "oocError.h"
#include "oocModifiers.h"
#include "oocObject.h"
#include "oocObject.r"
#include <stddef.h>

static OOC_AbstractBlockDeviceClass* AbstractBlockDeviceClass;
static OOC_AbstractBlockDeviceClass AbstractBlockDeviceInstance;

size_t ooc_abstractBlockDeviceGetBlockSize(const void* self) {
    OOC_TYPE_CHECK(self, ooc_abstractBlockDeviceClass(), 0);
    const OOC_AbstractBlockDevice* device = self;
    return device->blockSize;
}

size_t ooc_abstractBlockDeviceGetTotalBlocks(const void* self) {
    OOC_TYPE_CHECK(self, ooc_abstractBlockDeviceClass(), 0);
    const OOC_AbstractBlockDevice* device = self;
    return device->totalBlocks;
}

OOC_Error ooc_abstractBlockDeviceWrite(const void* self, size_t block, const void* buffer) {
    if (!self || !buffer) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_abstractBlockDeviceClass(), OOC_ERROR_INVALID_OBJECT);
    const OOC_AbstractBlockDeviceClass* class = ooc_classOf(self);
    if (block >= ooc_abstractBlockDeviceGetTotalBlocks(self)) {
        return OOC_ERROR_OUT_OF_BOUNDS;
    }
    return class->write(self, block, buffer);
}

OOC_Error ooc_abstractBlockDeviceRead(const void* self, size_t block, void* buffer) {
    if (!self || !buffer) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_abstractBlockDeviceClass(), OOC_ERROR_INVALID_OBJECT);
    const OOC_AbstractBlockDeviceClass* class = ooc_classOf(self);
    if (block >= ooc_abstractBlockDeviceGetTotalBlocks(self)) {
        return OOC_ERROR_OUT_OF_BOUNDS;
    }
    return class->read(self, block, buffer);
}

static OOC_Error ooc_abstractBlockDeviceCtor(void* self, va_list* args) {
    OOC_TYPE_CHECK(self, ooc_abstractBlockDeviceClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(ooc_abstractBlockDeviceClass(), self, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    OOC_AbstractBlockDevice* device = self;
    device->totalBlocks = va_arg(*args, size_t);
    device->blockSize = va_arg(*args, size_t);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_abstractBlockDeviceDtor(void* self) {
    OOC_TYPE_CHECK(self, ooc_abstractBlockDeviceClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_AbstractBlockDevice* device = self;
    device->totalBlocks = 0;
    device->blockSize = 0;
    return ooc_superDtor(ooc_abstractBlockDeviceClass(), self);
}

static void* ooc_abstractBlockClassInit(void) {
    if (ooc_classNew(&AbstractBlockDeviceInstance,
                    "AbstractBlockDevice",
                    sizeof(OOC_AbstractBlockDevice),
                    sizeof(OOC_AbstractBlockDeviceClass),
                    ooc_objectClass(),
                    OOC_MODIFIER_ABSTRACT,
                    OOC_METHOD_CTOR, ooc_abstractBlockDeviceCtor,
                    OOC_METHOD_DTOR, ooc_abstractBlockDeviceDtor,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &AbstractBlockDeviceInstance;
}

void* ooc_abstractBlockDeviceClass(void) {
    if (!AbstractBlockDeviceClass) {
        AbstractBlockDeviceClass = ooc_abstractBlockClassInit();
    }
    return AbstractBlockDeviceClass;
}
