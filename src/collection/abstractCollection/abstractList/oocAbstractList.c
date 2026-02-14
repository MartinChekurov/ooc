#include "oocAbstractList.h"
#include "oocAbstractIterator.h"
#include "oocAbstractList.r"

#include "oocAbstractCollection.h"
#include "oocCollection.h"
#include "oocError.h"
#include "oocList.h"
#include "oocObject.h"
#include "oocObject.r"
#include "oocAbstractIterator.r"
#include "oocAbstractListIterator.h"
#include "oocAbstractListIterator.r"

#include <stddef.h>

typedef struct OOC_AbstractListIterator_ OOC_AbstractListIterator_;
typedef struct OOC_AbstractListIteratorClass_ OOC_AbstractListIteratorClass_;

struct OOC_AbstractListIterator_ {
    OOC_AbstractListIterator baseListIterator;
    void* list;
    size_t nextIndex;
    int lastReturnedIndex;
};

struct OOC_AbstractListIteratorClass_ {
    OOC_AbstractListIteratorClass class;
};

static OOC_AbstractListClass* AbstractListClass;
static OOC_AbstractListClass AbstractListClassInstance;
static OOC_InterfaceImpl AbstractListInterfaces[1];

static OOC_AbstractListIteratorClass_* AbstractListIteratorClass_;
static OOC_AbstractListIteratorClass_ AbstractListIteratorClassInstance_;

size_t ooc_abstractListSize(void* self) {
    return ooc_collectionSize(self);
}

bool ooc_abstractListIsEmpty(void* self) {
    return ooc_collectionIsEmpty(self);
}

bool ooc_abstractListContains(void* self, void* element) {
    return ooc_collectionContains(self, element);
}

bool ooc_abstractListContainsAll(void* self, void* other) {
    return ooc_collectionContainsAll(self, other);
}

OOC_Error ooc_abstractListAdd(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_abstractListClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_listInsertAt(self, ooc_collectionSize(self), element);
}

OOC_Error ooc_abstractListRemove(void* self, void* element) {
    return ooc_collectionRemove(self, element);
}

OOC_Error ooc_abstractListClear(void* self) {
    return ooc_collectionClear(self);
}

void* ooc_abstractListGetAt(void* self, size_t index) {
    return ooc_listGetAt(self, index);
}

OOC_Error ooc_abstractListSetAt(void* self, size_t index, void* element) {
    return ooc_listSetAt(self, index, element);
}

OOC_Error ooc_abstractListInsertAt(void* self, size_t index, void* element) {
    return ooc_listInsertAt(self, index, element);
}

OOC_Error ooc_abstractListRemoveAt(void* self, size_t index) {
    return ooc_listRemoveAt(self, index);
}

int ooc_abstractListIndexOf(void* self, void* element) {
    if (!self) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_abstractListClass(), -1);
    size_t size = ooc_collectionSize(self);
    for (size_t i = 0; i < size; ++i) {
        void* current = ooc_listGetAt(self, i);
        if (ooc_equals(current, element)) {
            return (int)i;
        }
    }
    return -1;
}

int ooc_abstractListLastIndexOf(void* self, void* element) {
    if (!self) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_abstractListClass(), -1);
    size_t size = ooc_collectionSize(self);
    for (int i = (int)size - 1; i >= 0; --i) {
        void* current = ooc_listGetAt(self, (size_t)i);
        if (ooc_equals(current, element)) {
            return i;
        }
    }
    return -1;
}

static bool ooc_abstractListIteratorHasNext_(void* self) {
    if (!self) {
        return false;
    }
    OOC_AbstractListIterator_* iterator = self;
    return iterator->nextIndex < ooc_collectionSize(iterator->list);
}

static void* ooc_abstractListIteratorNext_(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_AbstractListIterator_* iterator = self;
    if (iterator->nextIndex >= ooc_collectionSize(iterator->list)) {
        return NULL;
    }
    ooc_abstractListIteratorNext(iterator);
    void* value = ooc_listGetAt(iterator->list, iterator->nextIndex);
    iterator->lastReturnedIndex = iterator->nextIndex;
    iterator->nextIndex++;
    return value;
}

static OOC_Error ooc_abstractListIteratorRemove_(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_AbstractListIterator_* iterator = self;
    OOC_Error error = ooc_abstractListIteratorRemove(iterator);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    error = ooc_listRemoveAt(iterator->list, iterator->lastReturnedIndex);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    if (iterator->lastReturnedIndex < (int)iterator->nextIndex) {
        iterator->nextIndex--;
    }
    iterator->lastReturnedIndex = -1;
    return error;
}

static bool ooc_abstractListIteratorHasPrevious_(void* self) {
    OOC_AbstractListIterator_* it = self;
    return it && it->nextIndex > 0;
}

static void* ooc_abstractListIteratorPrevious_(void* self) {
    OOC_AbstractListIterator_* it = self;
    if (!it || it->nextIndex == 0) {
        return NULL;
    }
    ooc_abstractListIteratorPrevious(it);
    it->nextIndex--;
    void* value = ooc_listGetAt(it->list, it->nextIndex);
    it->lastReturnedIndex = it->nextIndex;
    OOC_AbstractIterator* base = (OOC_AbstractIterator*)it;
    base->canModify = true;
    return value;
}

static int ooc_abstractListIteratorNextIndex_(void* self) {
    OOC_AbstractListIterator_* it = self;
    if (!it) {
        return -1;
    }
    return (int)it->nextIndex;
}

static int ooc_abstractListIteratorPreviousIndex_(void* self) {
    OOC_AbstractListIterator_* it = self;
    if (!it) {
        return -1;
    }
    return (it->nextIndex == 0) ? -1 : (int)(it->nextIndex - 1);
}

static OOC_Error ooc_abstractListIteratorSet_(void* self, void* element) {
    OOC_AbstractListIterator_* it = self;
    if (!it) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_Error error = ooc_abstractListIteratorSet(it, element);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    if (it->lastReturnedIndex < 0) {
        return OOC_ERROR_INVALID_STATE;
    }
    return ooc_listSetAt(it->list, it->lastReturnedIndex, element);
}

static OOC_Error ooc_abstractListIteratorAdd_(void* self, void* element) {
    OOC_AbstractListIterator_* it = self;
    if (!it) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_Error error = ooc_abstractListIteratorAdd(it, element);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    error = ooc_listInsertAt(it->list, it->nextIndex, element);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    it->nextIndex++;
    it->lastReturnedIndex = -1;
    OOC_AbstractIterator* base = (OOC_AbstractIterator*)it;
    base->canModify = false;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_abstractListIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_AbstractListIterator_* iterator = self;
    OOC_Error error = ooc_superCtor(iterator, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    iterator->list = va_arg(*args, void*);
    if (!iterator->list) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(iterator->list, ooc_abstractListClass(), OOC_ERROR_INVALID_OBJECT);
    iterator->nextIndex = va_arg(*args, size_t);
    if (iterator->nextIndex > ooc_collectionSize(iterator->list)) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    iterator->lastReturnedIndex = -1;
    return OOC_ERROR_NONE;
}

static void* ooc_abstractListIteratorClassInit_(void) {
    if (ooc_classNew(&AbstractListIteratorClassInstance_,
                     "AbstractListIterator",
                     sizeof(OOC_AbstractListIterator),
                     sizeof(OOC_AbstractListIteratorClass),
                     ooc_abstractListIteratorClass(),
                     OOC_MODIFIER_FINAL,
                     OOC_METHOD_CTOR, ooc_abstractListIteratorCtor,
                     OOC_ABSTRACT_ITERATOR_METHOD_HAS_NEXT, ooc_abstractListIteratorHasNext_,
                     OOC_ABSTRACT_ITERATOR_METHOD_NEXT, ooc_abstractListIteratorNext_,
                     OOC_ABSTRACT_ITERATOR_METHOD_REMOVE, ooc_abstractListIteratorRemove_,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_HAS_PREVIOUS, ooc_abstractListIteratorHasPrevious_,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_PREVIOUS, ooc_abstractListIteratorPrevious_,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_NEXT_INDEX, ooc_abstractListIteratorNextIndex_,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_PREVIOUS_INDEX, ooc_abstractListIteratorPreviousIndex_,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_SET, ooc_abstractListIteratorSet_,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_ADD, ooc_abstractListIteratorAdd_,
                     0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&AbstractListIteratorClassInstance_);
        return NULL;
    }
    return &AbstractListIteratorClassInstance_;
}

static void* ooc_abstractListIteratorClass_(void) {
    if (!AbstractListIteratorClass_) {
        AbstractListIteratorClass_ = ooc_abstractListIteratorClassInit_();
    }
    return AbstractListIteratorClass_;
}

void* ooc_abstractListGetIterator(void* self) {
    return ooc_abstractListGetListIterator(self);
}

void* ooc_abstractListGetListIterator(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_abstractListClass(), NULL);
    return ooc_new(ooc_abstractListIteratorClass_(), self, 0, 0);
}

void* ooc_abstractListGetListIteratorAt(void* self, size_t index) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_abstractListClass(), NULL);
    if (index > ooc_collectionSize(self)) {
        return NULL;
    }
    return ooc_new(ooc_abstractListIteratorClass_(), self, index, 0);
}

static void* ooc_abstractListClassInit(void) {
    if (ooc_classNew(&AbstractListClassInstance,
                     "AbstractList",
                     sizeof(OOC_AbstractList),
                     sizeof(OOC_AbstractListClass),
                     ooc_abstractCollectionClass(),
                     OOC_MODIFIER_ABSTRACT,
                     OOC_ABSTRACT_COLLECTION_METHOD_ITERATOR, ooc_abstractListGetIterator,
                     OOC_ABSTRACT_COLLECTION_METHOD_ADD, ooc_abstractListAdd,
                     OOC_ABSTRACT_LIST_METHOD_SET_AT, ooc_abstractListSetAt,
                     OOC_ABSTRACT_LIST_METHOD_INSERT_AT, ooc_abstractListInsertAt,
                     OOC_ABSTRACT_LIST_METHOD_REMOVE_AT, ooc_abstractListRemoveAt,
                     OOC_ABSTRACT_LIST_METHOD_INDEX_OF, ooc_abstractListIndexOf,
                     OOC_ABSTRACT_LIST_METHOD_LAST_INDEX_OF, ooc_abstractListLastIndexOf,
                     OOC_ABSTRACT_LIST_METHOD_GET_LIST_ITERATOR, ooc_abstractListGetListIterator,
                     OOC_ABSTRACT_LIST_METHOD_GET_LIST_ITERATOR_AT, ooc_abstractListGetListIteratorAt,
                     0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&AbstractListClassInstance);
        return NULL;
    }

    AbstractListInterfaces[0].interfaceClass = ooc_listClass();
    AbstractListInterfaces[0].vtableOffset = offsetof(OOC_AbstractListClass, listVtable);

    if (ooc_classSetInterface(&AbstractListClassInstance,
                              AbstractListInterfaces,
                              1) != OOC_ERROR_NONE) {
        ooc_classDestroy(&AbstractListClassInstance);
        return NULL;
    }

    return &AbstractListClassInstance;
}

void* ooc_abstractListClass(void) {
    if (!AbstractListClass) {
        AbstractListClass = ooc_abstractListClassInit();
    }
    return AbstractListClass;
}
