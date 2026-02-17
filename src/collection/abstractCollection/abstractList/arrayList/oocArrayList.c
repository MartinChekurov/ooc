#include "oocArrayList.h"
#include "oocAbstractList.r"
#include "oocAbstractList.h"
#include "oocArrayList.r"
#include "oocCollection.h"
#include "oocError.h"
#include "oocList.h"
#include "oocObject.h"
#include "oocAbstractIterator.r"
#include "oocGC.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define OOC_ARRAYLIST_DEFAULT_CAPACITY 10
#define OOC_ARRAYLIST_GROWTH_FACTOR 2

typedef struct OOC_ArrayListIterator OOC_ArrayListIterator;
typedef struct OOC_ArrayListIteratorClass OOC_ArrayListIteratorClass;

struct OOC_ArrayListIterator {
    OOC_AbstractIterator object;
    OOC_ArrayList* list;
    size_t currentIndex;
};

struct OOC_ArrayListIteratorClass {
    OOC_AbstractIteratorClass class;
};

static OOC_ArrayListClass* ArrayListClass;
static OOC_ArrayListClass ArrayListClassInstance;

static OOC_ArrayListIteratorClass* ArrayListIteratorClass;
static OOC_ArrayListIteratorClass ArrayListIteratorClassInstance;

static OOC_InterfaceImpl ArrayListInterfaces[1];

void* ooc_arrayListGetIterator(void* self) {
    return ooc_listGetIterator(self);
}

size_t ooc_arrayListSize(void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), 0);
    OOC_ArrayList* list = self;
    return list->size;
}

bool ooc_arrayListIsEmpty(void* self) {
    return ooc_collectionIsEmpty(self);
}

bool ooc_arrayListContains(void* self, void* element) {
    return ooc_collectionContains(self, element);
}

bool ooc_arrayListContainsAll(void* self, void* other) {
    return ooc_collectionContainsAll(self, other);
}

OOC_Error ooc_arrayListAdd(void* self, void* element) {
    return ooc_collectionAdd(self, element);
}

OOC_Error ooc_arrayListRemove(void* self, void* element) {
    return ooc_collectionRemove(self, element);
}

OOC_Error ooc_arrayListClear(void* self) {
    return ooc_collectionClear(self);
}

void* ooc_arrayListGetAt(void* self, size_t index) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), NULL);
    OOC_ArrayList* list = self;
    if (index >= list->size) {
        return NULL;
    }
    return list->elements[index];
}

OOC_Error ooc_arrayListSetAt(void* self, size_t index, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayList* list = self;
    if (index >= list->size) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    list->elements[index] = element;
    return OOC_ERROR_NONE;
}

OOC_Error ooc_arrayListInsertAt(void* self, size_t index, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayList* list = self;
    if (index > list->size) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_Error error = ooc_arrayListEnsureCapacity(list, list->size + 1);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    memmove(&list->elements[index + 1],
            &list->elements[index],
            (list->size - index) * sizeof(*list->elements));
    list->elements[index] = element;
    list->size++;
    return OOC_ERROR_NONE;
}

OOC_Error ooc_arrayListRemoveAt(void* self, size_t index) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayList* list = self;
    if (index >= list->size) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    if (index < list->size - 1) {
        memmove(list->elements + index,
                list->elements + index + 1,
                (list->size - index - 1) * sizeof(*list->elements));
    }
    list->size--;
    return OOC_ERROR_NONE;
}

int ooc_arrayListIndexOf(void* self, void* element) {
    return ooc_listIndexOf(self, element);
}

int ooc_arrayListLastIndexOf(void* self, void* element) {
    return ooc_listLastIndexOf(self, element);
}

void* ooc_arrayListGetListIterator(void* self) {
    return ooc_listGetListIterator(self);
}

void* ooc_arrayListGetListIteratorAt(void* self, size_t index) {
    return ooc_listGetListIteratorAt(self, index);
}

size_t ooc_arrayListCapacity(void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), 0);
    OOC_ArrayList* list = self;
    return list->capacity;
}

OOC_Error ooc_arrayListEnsureCapacity(void* self, size_t minCapacity) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayList* list = self;
    if (list->capacity >= minCapacity) {
        return OOC_ERROR_NONE;
    }
    size_t newCapacity = list->capacity * OOC_ARRAYLIST_GROWTH_FACTOR;
    if (newCapacity < minCapacity) {
        newCapacity = minCapacity;
    }
    void** newElements = realloc(list->elements, newCapacity * sizeof(void*));
    if (!newElements) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    list->elements = newElements;
    list->capacity = newCapacity;
    return OOC_ERROR_NONE;
}

OOC_Error ooc_arrayListTrimToSize(void* self) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayList* list = self;
    if (!list->size) {
        free(list->elements);
        list->elements = NULL;
        list->capacity = 0;
    } else {
        void** newElements = realloc(list->elements, list->size * sizeof(void*));
        if (!newElements) {
            return OOC_ERROR_OUT_OF_MEMORY;
        }
        list->elements = newElements;
        list->capacity = list->size;
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_arrayListCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayList* list = self;
    OOC_Error error = ooc_superCtor(ooc_arrayListClass(), list, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    list->capacity = va_arg(*args, size_t);
    if (!list->capacity) {
        list->capacity = OOC_ARRAYLIST_DEFAULT_CAPACITY;
    }
    list->size = 0;
    list->elements = calloc(list->capacity, sizeof(void*));
    if (!list->elements) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_arrayListDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayList* list = self;
    OOC_Error error = ooc_collectionClear(self);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    free(list->elements);
    list->elements = NULL;
    list->size = 0;
    list->capacity = 0;
    return ooc_superDtor(ooc_arrayListClass(), self);
}

static void ooc_arrayListGcMark(void* self, void* gc) {
    OOC_ArrayList* list = self;
    for (size_t i = 0; i < list->size; i++)
        ooc_gcMark(list->elements[i]);
}

static void* ooc_arrayListClassInit(void) {
    if (ooc_classNew(&ArrayListClassInstance,
                    "ArrayList",
                    sizeof(OOC_ArrayList),
                    sizeof(OOC_ArrayListClass),
                    ooc_abstractListClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_arrayListCtor,
                    OOC_METHOD_DTOR, ooc_arrayListDtor,
                    OOC_METHOD_GC_MARK, ooc_arrayListGcMark,
                    OOC_ABSTRACT_COLLECTION_METHOD_SIZE, ooc_arrayListSize,
                    OOC_ABSTRACT_LIST_METHOD_GET_AT, ooc_arrayListGetAt,
                    OOC_ABSTRACT_LIST_METHOD_SET_AT, ooc_arrayListSetAt,
                    OOC_ABSTRACT_LIST_METHOD_INSERT_AT, ooc_arrayListInsertAt,
                    OOC_ABSTRACT_LIST_METHOD_REMOVE_AT, ooc_arrayListRemoveAt,
                    0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&ArrayListClassInstance);
        return NULL;
    }
    return &ArrayListClassInstance;
}

void* ooc_arrayListClass() {
    if (!ArrayListClass) {
        ArrayListClass = ooc_arrayListClassInit();
    }
    return ArrayListClass;
}
