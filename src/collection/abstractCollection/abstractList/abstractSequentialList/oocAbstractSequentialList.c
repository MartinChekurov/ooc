#include "oocAbstractSequentialList.h"
#include "oocAbstractList.h"
#include "oocAbstractList.r"
#include "oocAbstractSequentialList.r"
#include "oocCollection.h"
#include "oocError.h"
#include "oocIterable.h"
#include "oocList.h"
#include "oocListIterator.h"
#include "oocObject.h"
#include "oocObject.r"

static OOC_AbstractSequentialListClass* AbstractSequentialListClass;
static OOC_AbstractSequentialListClass AbstractSequentialListClassInstance;

void* ooc_abstractSequantialListGetIterator(void* self) {
    return ooc_iterableGetIterator(self);
}

size_t ooc_abstractSequentialListSize(void* self) {
    return ooc_collectionSize(self);
}

bool ooc_abstractSequentialListIsEmpty(void* self) {
    return ooc_collectionIsEmpty(self);
}

bool ooc_abstractSequentialListContains(void* self, void* element) {
    return ooc_collectionContains(self, element);
}

bool ooc_abstractSequentialListContainsAll(void* self, void* other) {
    return ooc_collectionContainsAll(self, other);
}

OOC_Error ooc_abstractSequentialListAdd(void* self, void* element) {
    return ooc_collectionAdd(self, element);
}

OOC_Error ooc_abstractSequentialListRemove(void* self, void* element) {
    return ooc_collectionRemove(self, element);
}

OOC_Error ooc_abstractSequentialListClear(void* self) {
    return ooc_collectionClear(self);
}

void* ooc_abstractSequentialListGetAt(void* self, size_t index) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_abstractSequentialListClass(), NULL);
    void* it = ooc_listGetListIteratorAt(self, index);
    if (!it) {
        return NULL;
    }
    if (!ooc_listIteratorHasNext(it)) {
        ooc_destroy(it);
        return NULL;
    }
    void* value = ooc_listIteratorNext(it);
    ooc_destroy(it);
    return value;
}

OOC_Error ooc_abstractSequentialListSetAt(void* self, size_t index, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_abstractSequentialListClass(), OOC_ERROR_INVALID_OBJECT);
    void* it = ooc_listGetListIteratorAt(self, index);
    if (!it) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    if (!ooc_listIteratorHasNext(it)) {
        ooc_destroy(it);
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    (void)ooc_listIteratorNext(it);
    OOC_Error err = ooc_listIteratorSet(it, element);
    ooc_destroy(it);
    return err;
}

OOC_Error ooc_abstractSequentialListInsertAt(void* self, size_t index, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_abstractSequentialListClass(), OOC_ERROR_INVALID_OBJECT);
    void* it = ooc_listGetListIteratorAt(self, index);
    if (!it) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_Error err = ooc_listIteratorAdd(it, element);
    ooc_destroy(it);
    return err;  
}

OOC_Error ooc_abstractSequentialListRemoveAt(void* self, size_t index) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_abstractSequentialListClass(), OOC_ERROR_INVALID_OBJECT);
    void* it = ooc_listGetListIteratorAt(self, index);
    if (!it) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    if (!ooc_listIteratorHasNext(it)) {
        ooc_destroy(it);
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    (void)ooc_listIteratorNext(it);
    OOC_Error err = ooc_listIteratorRemove(it);
    ooc_destroy(it);
    return err;
}

int ooc_abstractSequentialListIndexOf(void* self, void* element) {
    return ooc_listIndexOf(self, element);
}

int ooc_abstractSequentialListLastIndexOf(void* self, void* element) {
    return ooc_listLastIndexOf(self, element);
}

void* ooc_abstractSequentialListGetListIterator(void* self) {
    return ooc_listGetListIterator(self);
}

void* ooc_abstractSequentialListGetListIteratorAt(void* self, size_t index) {
    return ooc_listGetListIteratorAt(self, index);
}

static void* ooc_abstractSequentialListClassInit(void) {
    if (ooc_classNew(&AbstractSequentialListClassInstance,
                     "AbstractSequentialList",
                     sizeof(OOC_AbstractSequentialList),
                     sizeof(OOC_AbstractSequentialListClass),
                     ooc_abstractListClass(),
                     OOC_MODIFIER_ABSTRACT,
                     OOC_ABSTRACT_LIST_METHOD_GET_AT, ooc_abstractSequentialListGetAt,
                     OOC_ABSTRACT_LIST_METHOD_SET_AT, ooc_abstractSequentialListSetAt,
                     OOC_ABSTRACT_LIST_METHOD_INSERT_AT, ooc_abstractSequentialListInsertAt,
                     OOC_ABSTRACT_LIST_METHOD_REMOVE_AT, ooc_abstractSequentialListRemoveAt,
                     OOC_ABSTRACT_LIST_METHOD_GET_LIST_ITERATOR_AT, NULL,
                     0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&AbstractSequentialListClassInstance);
        return NULL;
    }
    return &AbstractSequentialListClassInstance;
}

void* ooc_abstractSequentialListClass(void) {
    if (!AbstractSequentialListClass) {
        AbstractSequentialListClass = ooc_abstractSequentialListClassInit();
    }
    return AbstractSequentialListClass;
}
