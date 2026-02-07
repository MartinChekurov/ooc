#include "oocCollection.h"
#include "oocCollection.r"
#include "oocIterable.h"
#include "oocObject.h"
#include <stddef.h>

static OOC_CollectionClass* CollectionClass;
static OOC_CollectionClass CollectionClassInstance;

static void* ooc_collectionClassInit(void) {
    if (ooc_classNew(&CollectionClassInstance,
                    "Collection",
                    (size_t)0,
                    sizeof(OOC_CollectionClass),
                    ooc_iterableClass(),
                    OOC_MODIFIER_ABSTRACT,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &CollectionClassInstance;
}

void* ooc_collectionClass() {
    if (!CollectionClass) {
        CollectionClass = ooc_collectionClassInit();
    }
    return CollectionClass;
}

void* ooc_collectionGetIterator(void* self) {
    return ooc_iterableGetIterator(self);
}

size_t ooc_collectionSize(void* self) {
    if (!self) {
        return 0;
    }
    const OOC_CollectionVtable* vtable = ooc_getInterfaceVtable(self, ooc_collectionClass());
    if (!vtable) {
        return 0;
    }
    if (!vtable->size) {
        return 0;
    }
    return vtable->size(self);
}

bool ooc_collectionIsEmpty(void* self) {
    if (!self) {
        return true;
    }
    const OOC_CollectionVtable* vtable = ooc_getInterfaceVtable(self, ooc_collectionClass());
    if (!vtable) {
        return true;
    }
    if (!vtable->isEmpty) {
        return true;
    }
    return vtable->isEmpty(self);
}

bool ooc_collectionContains(void* self, void* element) {
    if (!self) {
        return false;
    }
    const OOC_CollectionVtable* vtable = ooc_getInterfaceVtable(self, ooc_collectionClass());
    if (!vtable) {
        return false;
    }
    if (!vtable->contains) {
        return false;
    }
    return vtable->contains(self, element);
}

bool ooc_collectionContainsAll(void* self, void* other) {
    if (!self || !other) {
        return false;
    }
    const OOC_CollectionVtable* vtable = ooc_getInterfaceVtable(self, ooc_collectionClass());
    if (!vtable) {
        return false;
    }
    if (!vtable->containsAll) {
        return false;
    }
    return vtable->containsAll(self, other);
}

OOC_Error ooc_collectionAdd(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    const OOC_CollectionVtable* vtable = ooc_getInterfaceVtable(self, ooc_collectionClass());
    if (!vtable) {
        return OOC_ERROR_NO_CLASS;
    }
    if (!vtable->add) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->add(self, element);
}

OOC_Error ooc_collectionRemove(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_CollectionVtable* vtable = ooc_getInterfaceVtable(self, ooc_collectionClass());
    if (!vtable) {
        return OOC_ERROR_NO_CLASS;
    }
    if (!vtable->remove) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->remove(self, element);
}

OOC_Error ooc_collectionClear(void* self) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    const OOC_CollectionVtable* vtable = ooc_getInterfaceVtable(self, ooc_collectionClass());
    if (!vtable) {
        return OOC_ERROR_NO_CLASS;
    }
    if (!vtable->clear) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->clear(self);
}