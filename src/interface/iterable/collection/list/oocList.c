#include "oocList.h"
#include "oocList.r"
#include "oocCollection.h"
#include "oocObject.h"
#include "oocIterable.h"
#include <stddef.h>

static OOC_ListClass* ListClass;
static OOC_ListClass ListClassInstance;

static void* ooc_listClassInit(void) {
    if (ooc_classNew(&ListClassInstance,
                    "List",
                    (size_t)0,
                    sizeof(OOC_ListClass),
                    ooc_collectionClass(),
                    OOC_MODIFIER_ABSTRACT,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &ListClassInstance;
}

void* ooc_listClass() {
    if (!ListClass) {
        ListClass = ooc_listClassInit();
    }
    return ListClass;
}

void* ooc_listGetIterator(void* self) {
    return ooc_iterableGetIterator(self);
}

size_t ooc_listSize(void* self) {
    return ooc_collectionSize(self);
}

bool ooc_listIsEmpty(void* self) {
    return ooc_collectionIsEmpty(self);
}

bool ooc_listContains(void* self, void* element) {
    return ooc_collectionContains(self, element);
}

OOC_Error ooc_listAdd(void* self, void* element) {
    return ooc_collectionAdd(self, element);
}

OOC_Error ooc_listRemove(void* self, void* element) {
    return ooc_collectionRemove(self, element);
}

OOC_Error ooc_listClear(void* self) {
    return ooc_collectionClear(self);
}

void* ooc_listGetAt(void* self, size_t index) {
    if (!self) {
        return NULL;
    }
    const OOC_ListVtable* vtable = ooc_getInterfaceVtable(self, ooc_listClass());
    if (!vtable) {
        return NULL;
    }
    if (!vtable->getAt) {
        return NULL;
    }
    return vtable->getAt(self, index);
}

OOC_Error ooc_listSetAt(void* self, size_t index, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    const OOC_ListVtable* vtable = ooc_getInterfaceVtable(self, ooc_listClass());
    if (!vtable) {
        return OOC_ERROR_NO_CLASS;
    }
    if (!vtable->setAt) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->setAt(self, index, element);
}

OOC_Error ooc_listInsertAt(void* self, size_t index, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    const OOC_ListVtable* vtable = ooc_getInterfaceVtable(self, ooc_listClass());
    if (!vtable) {
        return OOC_ERROR_NO_CLASS;
    }
    if (!vtable->insertAt) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->insertAt(self, index, element);
}

OOC_Error ooc_listRemoveAt(void* self, size_t index) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_ListVtable* vtable = ooc_getInterfaceVtable(self, ooc_listClass());
    if (!vtable) {
        return OOC_ERROR_NO_CLASS;
    }
    if (!vtable->removeAt) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->removeAt(self, index);
}

int ooc_listIndexOf(void* self, void* element) {
    if (!self) {
        return -1;
    }
    const OOC_ListVtable* vtable = ooc_getInterfaceVtable(self, ooc_listClass());
    if (!vtable) {
        return -1;
    }
    if (!vtable->indexOf) {
        return -1;
    }
    return vtable->indexOf(self, element);
}

int ooc_listLastIndexOf(void* self, void* element) {
    if (!self) {
        return -1;
    }
    const OOC_ListVtable* vtable = ooc_getInterfaceVtable(self, ooc_listClass());
    if (!vtable) {
        return -1;
    }
    if (!vtable->lastIndexOf) {
        return -1;
    }
    return vtable->lastIndexOf(self, element);
}