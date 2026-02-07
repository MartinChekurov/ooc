#include "oocArrayList.h"
#include "oocArrayList.r"
#include "oocList.h"
#include "oocList.r"
#include "oocCollection.r"
#include "oocIterable.r"
#include "oocCollection.h"
#include "oocIterable.h"
#include "oocIterator.h"
#include "oocIterator.r"
#include "oocObject.h"
#include "oocBaseIterator.h"
#include "oocBaseIterator.r"
#include "oocAbstractCollection.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define OOC_ARRAYLIST_DEFAULT_CAPACITY 10
#define OOC_ARRAYLIST_GROWTH_FACTOR 2

typedef struct OOC_ArrayListIterator OOC_ArrayListIterator;
typedef struct OOC_ArrayListIteratorClass OOC_ArrayListIteratorClass;

struct OOC_ArrayListIterator {
    OOC_BaseIterator object;
    OOC_ArrayList* list;
    size_t currentIndex;
};

struct OOC_ArrayListIteratorClass {
    OOC_BaseIteratorClass class;
};

static OOC_ArrayListClass* ArrayListClass;
static OOC_ArrayListClass ArrayListClassInstance;

static OOC_ArrayListIteratorClass* ArrayListIteratorClass;
static OOC_ArrayListIteratorClass ArrayListIteratorClassInstance;

static OOC_InterfaceImpl ArrayListInterfaces[1];

static size_t ooc_arrayListSize(void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), 0);
    OOC_ArrayList* list = self;
    return list->size;
}

static bool ooc_arrayListIsEmpty(void* self) {
    if (!self) {
        return true;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), true);
    OOC_ArrayList* list = self;
    return list->size == 0;
}

static void* ooc_arrayListGet(void* self, void* element) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), NULL);
    OOC_ArrayList* list = self;
    for (size_t i = 0; i < list->size; i++) {
        if (ooc_equals(list->elements[i], element)) {
            return list->elements[i];
        }
    }
    return NULL;
}

static bool ooc_arrayListContains(void* self, void* element) {
    return ooc_arrayListGet(self, element) != NULL;
}

static bool ooc_arrayListContainsAll(void* self, void* other) {
    if (!self || !other) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), false);
    OOC_TYPE_CHECK(other, ooc_iterableClass(), false);
    bool result = true;
    void* iterator = ooc_iterableGetIterator(other);
    while (ooc_iteratorHasNext(iterator)) {
        void* element = ooc_iteratorNext(iterator);
        if (!ooc_arrayListContains(self, element)) {
            result = false;
        }
    }
    ooc_destroy(iterator);
    return result;
}

static OOC_Error ooc_arrayListAdd(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayList* list = self;
    OOC_Error error = ooc_arrayListEnsureCapacity(list, list->size + 1);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    list->elements[list->size++] = element;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_arrayListRemove(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayList* list = self;
    for (size_t i = 0; i < list->size; i++) {
        if (ooc_equals(list->elements[i], element)) {
            ooc_destroy(list->elements[i]);
            if (i != list->size - 1) {
                memmove(list->elements + i,
                        list->elements + i + 1,
                        (list->size - i - 1) * sizeof(*list->elements));
            }
            list->size--;
            return OOC_ERROR_NONE;
        }
    }
    return OOC_ERROR_NOT_FOUND;
}

static OOC_Error ooc_arrayListClear(void* self) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayList* list = self;
    for (size_t i = 0; i < list->size; i++) {
        ooc_destroy(list->elements[i]);
        list->elements[i] = NULL;
    }
    list->size = 0;
    return OOC_ERROR_NONE;
}

static void* ooc_arrayListGetAt(void* self, size_t index) {
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

static OOC_Error ooc_arrayListSetAt(void* self, size_t index, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayList* list = self;
    if (index >= list->size) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    ooc_destroy(list->elements[index]);
    list->elements[index] = element;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_arrayListInsertAt(void* self, size_t index, void* element) {
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

static OOC_Error ooc_arrayListRemoveAt(void* self, size_t index) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayList* list = self;
    if (index >= list->size) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    ooc_destroy(list->elements[index]);
    if (index < list->size - 1) {
        memmove(list->elements + index,
                list->elements + index + 1,
                (list->size - index - 1) * sizeof(*list->elements));
    }
    list->size--;
    return OOC_ERROR_NONE;
}

static int ooc_arrayListIndexOf(void* self, void* element) {
    if (!self) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), -1);
    OOC_ArrayList* list = self;
    for (size_t i = 0; i < list->size; i++) {
        if (ooc_equals(list->elements[i], element)) {
            return (int)i;
        }
    }
    return -1;
}

static int ooc_arrayListLastIndexOf(void* self, void* element) {
    if (!self) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), -1);
    OOC_ArrayList* list = self;
    for (int i = (int)list->size - 1; i >= 0; i--) {
        if (ooc_equals(list->elements[i], element)) {
            return i;
        }
    }
    return -1;
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

static bool ooc_arrayListIteratorHasNext(void* self) {
    if (!self) {
        return false;
    }
    OOC_ArrayListIterator* iterator = self;
    return iterator->currentIndex < iterator->list->size;
}

static void* ooc_arrayListIteratorNext(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_ArrayListIterator* iterator = self;
    if (iterator->currentIndex >= iterator->list->size) {
        return NULL;
    }
    ooc_superBaseIteratorNext(iterator);
    return iterator->list->elements[iterator->currentIndex++];
}

static OOC_Error ooc_arrayListIteratorRemove(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_ArrayListIterator* iterator = self;
    OOC_Error error = ooc_superBaseIteratorRemove(iterator);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    if (!iterator->currentIndex || iterator->currentIndex > iterator->list->size) {
        return OOC_ERROR_NOT_FOUND;
    }
    size_t indexToRemove = iterator->currentIndex - 1;
    if (ooc_arrayListRemoveAt(iterator->list, indexToRemove) != OOC_ERROR_NONE) {
        return OOC_ERROR_NOT_FOUND;
    }
    iterator->currentIndex--;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_arrayListIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_ArrayListIterator* iterator = self;
    OOC_Error error = ooc_superCtor(iterator, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    iterator->list = va_arg(*args, OOC_ArrayList*);
    if (!iterator->list) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    iterator->currentIndex = 0;
    return OOC_ERROR_NONE;
}

static void* ooc_arrayListIteratorClassInit(void) {
    if (ooc_classNew(&ArrayListIteratorClassInstance,
                    "ArrayListIterator",
                    sizeof(OOC_ArrayListIterator),
                    sizeof(OOC_ArrayListIteratorClass),
                    ooc_baseIteratorClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_arrayListIteratorCtor,
                    0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&ArrayListIteratorClassInstance);
        return NULL;
    }
    OOC_IteratorVtable* vtable = (void*)ooc_classGetInterfaceVtable(&ArrayListIteratorClassInstance, ooc_iteratorClass());
    if (vtable) {
        vtable->remove = ooc_arrayListIteratorRemove;
        vtable->hasNext = ooc_arrayListIteratorHasNext;
        vtable->next = ooc_arrayListIteratorNext;
    }
    return &ArrayListIteratorClassInstance;
}

static void* ooc_arrayListIteratorClass(void) {
    if (!ArrayListIteratorClass) {
        ArrayListIteratorClass = ooc_arrayListIteratorClassInit();
    }
    return ArrayListIteratorClass;
}

void* ooc_arrayListGetIterator(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), NULL);
    OOC_ArrayList* list = self;
    return ooc_new(ooc_arrayListIteratorClass(), list, 0);
}

static OOC_Error ooc_arrayListCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_arrayListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayList* list = self;
    OOC_Error error = ooc_superCtor(list, args);
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
    OOC_Error error = ooc_arrayListClear(self);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    free(list->elements);
    list->elements = NULL;
    list->size = 0;
    list->capacity = 0;
    return ooc_superDtor(self);
}

static void* ooc_arrayListClassInit(void) {
    if (ooc_classNew(&ArrayListClassInstance,
                    "ArrayList",
                    sizeof(OOC_ArrayList),
                    sizeof(OOC_ArrayListClass),
                    ooc_abstractCollectionClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_arrayListCtor,
                    OOC_METHOD_DTOR, ooc_arrayListDtor,
                    0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&ArrayListClassInstance);
        return NULL;
    }
    ArrayListInterfaces[0].interfaceClass = ooc_listClass();
    ArrayListInterfaces[0].vtableOffset = offsetof(OOC_ArrayListClass, list);
    ArrayListClassInstance.list.collectionVtable.iterableVtable.iterator = ooc_arrayListGetIterator;
    ArrayListClassInstance.list.collectionVtable.size = ooc_arrayListSize;
    ArrayListClassInstance.list.collectionVtable.isEmpty = ooc_arrayListIsEmpty;
    ArrayListClassInstance.list.collectionVtable.contains = ooc_arrayListContains;
    ArrayListClassInstance.list.collectionVtable.containsAll = ooc_arrayListContainsAll;
    ArrayListClassInstance.list.collectionVtable.add = ooc_arrayListAdd;
    ArrayListClassInstance.list.collectionVtable.remove = ooc_arrayListRemove;
    ArrayListClassInstance.list.collectionVtable.clear = ooc_arrayListClear;
    ArrayListClassInstance.list.getAt = ooc_arrayListGetAt;
    ArrayListClassInstance.list.setAt = ooc_arrayListSetAt;
    ArrayListClassInstance.list.insertAt = ooc_arrayListInsertAt;
    ArrayListClassInstance.list.removeAt = ooc_arrayListRemoveAt;
    ArrayListClassInstance.list.indexOf = ooc_arrayListIndexOf;
    ArrayListClassInstance.list.lastIndexOf = ooc_arrayListLastIndexOf;
    if (ooc_classSetInterface(&ArrayListClassInstance,
                          ArrayListInterfaces,
                          1) != OOC_ERROR_NONE) {
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
