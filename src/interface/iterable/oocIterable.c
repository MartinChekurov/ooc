#include "oocIterable.h"
#include "oocIterable.r"
#include "oocInterface.h"
#include "oocObject.h"
#include <stddef.h>

static OOC_IterableClass* IterableClass;
static OOC_IterableClass IterableClassInstance;

static void* ooc_iterableClassInit(void) {
    if (ooc_classNew(&IterableClassInstance,
                    "Iterable",
                    (size_t)0,
                    sizeof(OOC_IterableClass),
                    ooc_interfaceClass(),
                    OOC_MODIFIER_ABSTRACT,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &IterableClassInstance;
}

void* ooc_iterableClass() {
    if (!IterableClass) {
        IterableClass = ooc_iterableClassInit();
    }
    return IterableClass;
}

void* ooc_iterableGetIterator(void* self) {
    if (!self) {
        return false;
    }
    const OOC_IterableVtable* vtable = ooc_getInterfaceVtable(self, ooc_iterableClass());
    if (!vtable || !vtable->iterator(self)) {
        return false;
    }
    return vtable->iterator(self);
}