#include "oocListIterator.h"
#include "oocListIterator.r"

#include "oocObject.h"
#include "oocObject.r"
#include "oocIterator.h"

static OOC_ListIteratorClass* ListIteratorClass;
static OOC_ListIteratorClass ListIteratorClassInstance;

static const OOC_ListIteratorVtable* ooc_listIteratorVtable(void* self) {
    if (!self) {
        return NULL;
    }
    return ooc_getInterfaceVtable(self, ooc_listIteratorClass());
}

bool ooc_listIteratorHasNext(void* self) {
    return ooc_iteratorHasNext(self);
}

void* ooc_listIteratorNext(void* self) {
    return ooc_iteratorNext(self);
}

OOC_Error ooc_listIteratorRemove(void* self) {
    return ooc_iteratorRemove(self);
}

bool ooc_listIteratorHasPrevious(void* self) {
    const OOC_ListIteratorVtable* vtable = ooc_listIteratorVtable(self);
    if (!vtable || !vtable->hasPrevious) {
        return false;
    }
    return vtable->hasPrevious(self);
}

void* ooc_listIteratorPrevious(void* self) {
    const OOC_ListIteratorVtable* vtable = ooc_listIteratorVtable(self);
    if (!vtable || !vtable->previous) {
        return NULL;
    }
    return vtable->previous(self);
}

int ooc_listIteratorNextIndex(void* self) {
    const OOC_ListIteratorVtable* vtable = ooc_listIteratorVtable(self);
    if (!vtable || !vtable->nextIndex) {
        return -1;
    }
    return vtable->nextIndex(self);
}

int ooc_listIteratorPreviousIndex(void* self) {
    const OOC_ListIteratorVtable* vtable = ooc_listIteratorVtable(self);
    if (!vtable || !vtable->previousIndex) {
        return -1;
    }
    return vtable->previousIndex(self);
}

OOC_Error ooc_listIteratorSet(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_ListIteratorVtable* vtable = ooc_listIteratorVtable(self);
    if (!vtable || !vtable->set) {
        return OOC_ERROR_NOT_SUPPORTED;
    }
    return vtable->set(self, element);
}

OOC_Error ooc_listIteratorAdd(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_ListIteratorVtable* vtable = ooc_listIteratorVtable(self);
    if (!vtable || !vtable->add) {
        return OOC_ERROR_NOT_SUPPORTED;
    }
    return vtable->add(self, element);
}

static void* ooc_listIteratorClassInit(void) {
    if (ooc_classNew(&ListIteratorClassInstance,
                     "ListIterator",
                     sizeof(OOC_Object),
                     sizeof(OOC_ListIteratorClass),
                     ooc_iteratorClass(),
                     OOC_MODIFIER_ABSTRACT,
                     0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&ListIteratorClassInstance);
        return NULL;
    }

    return &ListIteratorClassInstance;
}

void* ooc_listIteratorClass(void) {
    if (!ListIteratorClass) {
        ListIteratorClass = ooc_listIteratorClassInit();
    }

    return ListIteratorClass;
}
