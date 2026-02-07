#include "oocIterator.h"
#include "oocIterator.r"
#include "oocInterface.h"
#include "oocObject.h"
#include <stddef.h>

static OOC_IteratorClass* IteratorClass;
static OOC_IteratorClass IteratorClassInstance;

static void* ooc_iteratorClassInit(void) {
    if (ooc_classNew(&IteratorClassInstance,
                    "Iterator",
                    (size_t)0,
                    sizeof(OOC_IteratorClass),
                    ooc_interfaceClass(),
                    OOC_MODIFIER_ABSTRACT,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &IteratorClassInstance;
}

void* ooc_iteratorClass() {
    if (!IteratorClass) {
        IteratorClass = ooc_iteratorClassInit();
    }
    return IteratorClass;
}

bool ooc_iteratorHasNext(void* self) {
    if (!self) {
        return false;
    }
    const OOC_IteratorVtable* vtable = ooc_getInterfaceVtable(self, ooc_iteratorClass());
    if (!vtable || !vtable->hasNext) {
        return false;
    }
    return vtable->hasNext(self);
}

void* ooc_iteratorNext(void* self) {
    if (!self) {
        return false;
    }
    const OOC_IteratorVtable* vtable = ooc_getInterfaceVtable(self, ooc_iteratorClass());
    if (!vtable || !vtable->next) {
        return false;
    }
    return vtable->next(self);
}

OOC_Error ooc_iteratorRemove(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_IteratorVtable* vtable = ooc_getInterfaceVtable(self, ooc_iteratorClass());
    if (!vtable || !vtable->remove) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->remove(self);
}