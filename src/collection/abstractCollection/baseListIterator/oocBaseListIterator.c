#include "oocBaseListIterator.h"
#include "oocBaseListIterator.r"

#include "oocBaseIterator.h"
#include "oocListIterator.h"
#include "oocObject.h"
#include <stddef.h>

static OOC_BaseListIteratorClass* BaseListIteratorClass;
static OOC_BaseListIteratorClass BaseListIteratorClassInstance;

static OOC_InterfaceImpl BaseListIteratorInterfaces[1];

bool ooc_baseListIteratorHasPrevious(void* self) {
    (void)self;
    return false;
}

void* ooc_baseListIteratorPrevious(void* self) {
    (void)self;
    return NULL;
}

int ooc_baseListIteratorNextIndex(void* self) {
    (void)self;
    return -1;
}

int ooc_baseListIteratorPreviousIndex(void* self) {
    (void)self;
    return -1;
}

OOC_Error ooc_baseListIteratorSet(void* self, void* element) {
    (void)self;
    (void)element;
    return OOC_ERROR_NOT_SUPPORTED;
}

OOC_Error ooc_baseListIteratorAdd(void* self, void* element) {
    (void)self;
    (void)element;
    return OOC_ERROR_NOT_SUPPORTED;
}

static void* ooc_baseListIteratorClassInit(void) {
    if (ooc_classNew(&BaseListIteratorClassInstance,
                     "BaseListIterator",
                     sizeof(OOC_BaseListIterator),
                     sizeof(OOC_BaseListIteratorClass),
                     ooc_baseIteratorClass(),
                     OOC_MODIFIER_ABSTRACT,
                     OOC_BASE_LIST_ITERATOR_METHOD_HAS_PREVIOUS, ooc_baseListIteratorHasPrevious,
                     OOC_BASE_LIST_ITERATOR_METHOD_PREVIOUS, ooc_baseListIteratorPrevious,
                     OOC_BASE_LIST_ITERATOR_METHOD_NEXT_INDEX, ooc_baseListIteratorNextIndex,
                     OOC_BASE_LIST_ITERATOR_METHOD_PREVIOUS_INDEX, ooc_baseListIteratorPreviousIndex,
                     OOC_BASE_LIST_ITERATOR_METHOD_SET, ooc_baseListIteratorSet,
                     OOC_BASE_LIST_ITERATOR_METHOD_ADD, ooc_baseListIteratorAdd,
                     0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&BaseListIteratorClassInstance);
        return NULL;
    }

    BaseListIteratorInterfaces[0].interfaceClass = ooc_listIteratorClass();
    BaseListIteratorInterfaces[0].vtableOffset = offsetof(OOC_BaseListIteratorClass, listIteratorVtable);

    if (ooc_classSetInterface(&BaseListIteratorClassInstance,
                              BaseListIteratorInterfaces,
                              1) != OOC_ERROR_NONE) {
        ooc_classDestroy(&BaseListIteratorClassInstance);
        return NULL;
    }

    return &BaseListIteratorClassInstance;
}

void* ooc_baseListIteratorClass(void) {
    if (!BaseListIteratorClass) {
        BaseListIteratorClass = ooc_baseListIteratorClassInit();
    }
    return BaseListIteratorClass;
}
