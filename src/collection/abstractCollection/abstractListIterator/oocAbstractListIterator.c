#include "oocAbstractListIterator.h"
#include "oocAbstractListIterator.r"

#include "oocAbstractIterator.h"
#include "oocIterator.h"
#include "oocListIterator.h"
#include "oocObject.h"
#include <stddef.h>
#include <string.h>

static OOC_AbstractListIteratorClass* AbstractListIteratorClass;
static OOC_AbstractListIteratorClass AbstractListIteratorClassInstance;

static OOC_InterfaceImpl AbstractListIteratorInterfaces[1];

bool ooc_abstractListIteratorHasNext(void* self) {
    return ooc_iteratorHasNext(self);
}

void* ooc_abstractListIteratorNext(void* self) {
    return ooc_iteratorNext(self);
}
OOC_Error ooc_abstractListIteratorRemove(void* self) {
    return ooc_iteratorRemove(self);
}

bool ooc_abstractListIteratorHasPrevious(void* self) {
    return ooc_listIteratorHasPrevious(self);
}

void* ooc_abstractListIteratorPrevious(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_abstractListIteratorClass(), NULL);
    OOC_AbstractIterator* iterator = self;
    iterator->canModify = true;
    return NULL;
}

int ooc_abstractListIteratorNextIndex(void* self) {
    return ooc_listIteratorNextIndex(self);
}

int ooc_abstractListIteratorPreviousIndex(void* self) {
    return ooc_listIteratorPreviousIndex(self);
}

OOC_Error ooc_abstractListIteratorSet(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_abstractListIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_AbstractIterator* iterator = self;
    if (!iterator->canModify) {
        return OOC_ERROR_INVALID_STATE;
    }
    return OOC_ERROR_NONE;
}

OOC_Error ooc_abstractListIteratorAdd(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_abstractListIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_AbstractIterator* iterator = self;
    iterator->canModify = false;
    return OOC_ERROR_NONE;
}

static void* ooc_abstractListIteratorClassInit(void) {
    if (ooc_classNew(&AbstractListIteratorClassInstance,
                     "AbstractListIterator",
                     sizeof(OOC_AbstractListIterator),
                     sizeof(OOC_AbstractListIteratorClass),
                     ooc_abstractIteratorClass(),
                     OOC_MODIFIER_ABSTRACT,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_HAS_PREVIOUS, NULL,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_PREVIOUS, ooc_abstractListIteratorPrevious,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_NEXT_INDEX, NULL,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_PREVIOUS_INDEX, NULL,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_SET, ooc_abstractListIteratorSet,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_ADD, ooc_abstractListIteratorAdd,
                     0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&AbstractListIteratorClassInstance);
        return NULL;
    }

    AbstractListIteratorInterfaces[0].interfaceClass = ooc_listIteratorClass();
    AbstractListIteratorInterfaces[0].vtableOffset = offsetof(OOC_AbstractListIteratorClass, listIteratorVtable);

    if (ooc_classSetInterface(&AbstractListIteratorClassInstance,
                              AbstractListIteratorInterfaces,
                              1) != OOC_ERROR_NONE) {
        ooc_classDestroy(&AbstractListIteratorClassInstance);
        return NULL;
    }

    return &AbstractListIteratorClassInstance;
}

void* ooc_abstractListIteratorClass(void) {
    if (!AbstractListIteratorClass) {
        AbstractListIteratorClass = ooc_abstractListIteratorClassInit();
    }
    return AbstractListIteratorClass;
}
