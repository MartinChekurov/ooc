#include "oocArrayDeque.h"
#include "oocArrayDeque.r"

#include "oocAbstractCollection.h"
#include "oocCollection.h"
#include "oocAbstractIterator.h"
#include "oocAbstractIterator.r"
#include "oocDeque.h"
#include "oocError.h"
#include "oocObject.h"
#include "oocQueue.h"

#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define OOC_ARRAY_DEQUE_DEFAULT_CAPACITY 16

#define OOC_ARRAY_DEQUE_INCREMENT_INDEX(deque, index)\
    do {\
        index = (index + 1) % deque->capacity;\
    } while (0)

#define OOC_ARRAY_DEQUE_DECREMENT_INDEX(deque, index)\
    do {\
        index = (index + deque->capacity - 1) % deque->capacity;\
    } while (0)

#define OOC_ARRAY_DEQUE_GET_PHYSICAL_INDEX(deque, pointer, logicalIndex) ((pointer + logicalIndex) % deque->capacity)

typedef struct OOC_ArrayDequeIterator OOC_ArrayDequeIterator;
typedef struct OOC_ArrayDequeIteratorClass OOC_ArrayDequeIteratorClass;

struct OOC_ArrayDequeIterator {
    OOC_AbstractIterator baseIterator;
    OOC_ArrayDeque* deque;
    size_t index;
    size_t lastIndex;
};

struct OOC_ArrayDequeIteratorClass {
    OOC_AbstractIteratorClass class;
};

static OOC_ArrayDequeClass* ArrayDequeClass;
static OOC_ArrayDequeClass ArrayDequeClassInstance;
static OOC_InterfaceImpl ArrayDequeInterfaces[2];

static OOC_ArrayDequeIteratorClass* ArrayDequeIteratorClass;
static OOC_ArrayDequeIteratorClass ArrayDequeIteratorClassInstance;

static void* ooc_arrayDequeIteratorClass(void);

static OOC_Error ooc_arrayDequeEnsureCapacity(OOC_ArrayDeque* deque, size_t minCapacity) {
    if (!deque) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    if (deque->capacity >= minCapacity) {
        return OOC_ERROR_NONE;
    }
    size_t newCapacity = deque->capacity * 2;
    if (newCapacity < minCapacity) {
        newCapacity = minCapacity;
    }
    void** newElements = calloc(newCapacity, sizeof(void*));
    if (!newElements) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    for (size_t i = 0; i < deque->size; ++i) {
        size_t physicalIndex = OOC_ARRAY_DEQUE_GET_PHYSICAL_INDEX(deque, deque->head, i);
        newElements[i] = deque->elements[physicalIndex];
    }
    free(deque->elements);
    deque->elements = newElements;
    deque->capacity = newCapacity;
    deque->head = 0;
    deque->tail = deque->size;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_arrayDequeDeleteAt(OOC_ArrayDeque* deque, size_t index) {
    if (!deque) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    if (index >= deque->size) {
        return OOC_ERROR_OUT_OF_BOUNDS;
    }
    size_t physicalIndex = OOC_ARRAY_DEQUE_GET_PHYSICAL_INDEX(deque, deque->head, index);
    void* removed = deque->elements[physicalIndex];
    for (size_t i = index; i + 1 < deque->size; ++i) {
        size_t curr = OOC_ARRAY_DEQUE_GET_PHYSICAL_INDEX(deque, deque->head, i);
        size_t next = OOC_ARRAY_DEQUE_GET_PHYSICAL_INDEX(deque, deque->head, i + 1);
        deque->elements[curr] = deque->elements[next];
    }
    OOC_ARRAY_DEQUE_DECREMENT_INDEX(deque, deque->tail);
    deque->size--;
    ooc_destroy(removed);
    return OOC_ERROR_NONE;
}

static bool ooc_arrayDequeIteratorHasNext(void* self) {
    if (!self) {
        return false;
    }
    OOC_ArrayDequeIterator* iterator = self;
    return iterator->index < iterator->deque->size;
}

static void* ooc_arrayDequeIteratorNext(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_ArrayDequeIterator* iterator = self;
    if (!ooc_arrayDequeIteratorHasNext(iterator)) {
        return NULL;
    }
    ooc_abstractIteratorNext(iterator);
    size_t physicalIndex = OOC_ARRAY_DEQUE_GET_PHYSICAL_INDEX(iterator->deque, iterator->deque->head, iterator->index);
    void* element = iterator->deque->elements[physicalIndex];
    iterator->lastIndex = iterator->index;
    iterator->index++;
    return element;
}

static OOC_Error ooc_arrayDequeIteratorRemove(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_ArrayDequeIterator* iterator = self;
    OOC_Error error = ooc_abstractIteratorRemove(iterator);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    error = ooc_arrayDequeDeleteAt(iterator->deque, iterator->lastIndex);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    iterator->index--;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_arrayDequeIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_ArrayDequeIterator* iterator = self;
    OOC_Error error = ooc_superCtor(ooc_arrayDequeIteratorClass(), iterator, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    iterator->deque = va_arg(*args, OOC_ArrayDeque*);
    if (!iterator->deque) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(iterator->deque, ooc_arrayDequeClass(), OOC_ERROR_INVALID_OBJECT);
    iterator->index = 0;
    iterator->lastIndex = 0;
    return OOC_ERROR_NONE;
}

static void* ooc_arrayDequeIteratorClassInit(void) {
    if (ooc_classNew(&ArrayDequeIteratorClassInstance,
                     "ArrayDequeIterator",
                     sizeof(OOC_ArrayDequeIterator),
                     sizeof(OOC_ArrayDequeIteratorClass),
                     ooc_abstractIteratorClass(),
                     OOC_MODIFIER_NONE,
                     OOC_METHOD_CTOR, ooc_arrayDequeIteratorCtor,
                     OOC_ABSTRACT_ITERATOR_METHOD_HAS_NEXT, ooc_arrayDequeIteratorHasNext,
                     OOC_ABSTRACT_ITERATOR_METHOD_NEXT, ooc_arrayDequeIteratorNext,
                     OOC_ABSTRACT_ITERATOR_METHOD_REMOVE, ooc_arrayDequeIteratorRemove,
                     0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&ArrayDequeIteratorClassInstance);
        return NULL;
    }
    return &ArrayDequeIteratorClassInstance;
}

static void* ooc_arrayDequeIteratorClass(void) {
    if (!ArrayDequeIteratorClass) {
        ArrayDequeIteratorClass = ooc_arrayDequeIteratorClassInit();
    }
    return ArrayDequeIteratorClass;
}

void* ooc_arrayDequeGetIterator(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), NULL);
    return ooc_new(ooc_arrayDequeIteratorClass(), self, 0);
}

size_t ooc_arrayDequeSize(void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), 0);
    OOC_ArrayDeque* deque = (OOC_ArrayDeque*)self;
    return deque->size;
}

bool ooc_arrayDequeIsEmpty(void* self) {
    return ooc_collectionIsEmpty(self);
}

bool ooc_arrayDequeContains(void* self, void* element) {
    return ooc_collectionContains(self, element);
}

bool ooc_arrayDequeContainsAll(void* self, void* other) {
    return ooc_collectionContainsAll(self, other);
}

OOC_Error ooc_arrayDequeAdd(void* self, void* element) {
    return ooc_arrayDequeAddLast(self, element);
}

OOC_Error ooc_arrayDequeRemove(void* self, void* element) {
    return ooc_collectionRemove(self, element);
}

OOC_Error ooc_arrayDequeClear(void* self) {
    return ooc_collectionClear(self);
}

OOC_Error ooc_arrayDequeOffer(void* self, void* element) {
    return ooc_arrayDequeAddLast(self, element);
}

void* ooc_arrayDequePoll(void* self) {
    return ooc_arrayDequeRemoveFirst(self);
}

void* ooc_arrayDequePeek(void* self) {
    return ooc_arrayDequeGetFirst(self);
}

OOC_Error ooc_arrayDequeAddFirst(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayDeque* deque = self;
    OOC_Error error = ooc_arrayDequeEnsureCapacity(deque, deque->size + 1);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    OOC_ARRAY_DEQUE_DECREMENT_INDEX(deque, deque->head);
    deque->elements[deque->head] = element;
    deque->size++;
    return OOC_ERROR_NONE;
}

OOC_Error ooc_arrayDequeAddLast(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayDeque* deque = self;
    OOC_Error error = ooc_arrayDequeEnsureCapacity(deque, deque->size + 1);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    deque->elements[deque->tail] = element;
    OOC_ARRAY_DEQUE_INCREMENT_INDEX(deque, deque->tail);
    deque->size++;
    return OOC_ERROR_NONE;
}

void* ooc_arrayDequeGetFirst(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), NULL);
    OOC_ArrayDeque* deque = self;
    if (!deque->size) {
        return NULL;
    }
    return deque->elements[deque->head];
}

void* ooc_arrayDequeGetLast(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), NULL);
    OOC_ArrayDeque* deque = self;
    if (!deque->size) {
        return NULL;
    }
    size_t index = (deque->tail + deque->capacity - 1) % deque->capacity;
    return deque->elements[index];
}

void* ooc_arrayDequeRemoveFirst(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), NULL);
    OOC_ArrayDeque* deque = self;
    if (!deque->size) {
        return NULL;
    }
    void* value = deque->elements[deque->head];
    OOC_ARRAY_DEQUE_INCREMENT_INDEX(deque, deque->head);
    deque->size--;
    return value;
}

void* ooc_arrayDequeRemoveLast(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), NULL);
    OOC_ArrayDeque* deque = self;
    if (!deque->size) {
        return NULL;
    }
    OOC_ARRAY_DEQUE_DECREMENT_INDEX(deque, deque->tail);
    void* value = deque->elements[deque->tail];
    deque->size--;
    return value;
}

OOC_Error ooc_arrayDequePush(void* self, void* element) {
    return ooc_arrayDequeAddFirst(self, element);
}

void* ooc_arrayDequePop(void* self) {
    return ooc_arrayDequeRemoveFirst(self);
}

static OOC_Error ooc_arrayDequeCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayDeque* deque = self;
    OOC_Error error = ooc_superCtor(ooc_arrayDequeClass(), deque, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    deque->capacity = va_arg(*args, size_t);
    if (!deque->capacity) {
        deque->capacity = OOC_ARRAY_DEQUE_DEFAULT_CAPACITY;
    }
    deque->elements = calloc(deque->capacity, sizeof(void*));
    if (!deque->elements) {
        deque->capacity = 0;
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    deque->size = 0;
    deque->head = 0;
    deque->tail = 0;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_arrayDequeDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_collectionClear(self);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    OOC_ArrayDeque* deque = self;
    free(deque->elements);
    deque->elements = NULL;
    deque->capacity = 0;
    deque->size = 0;
    deque->head = 0;
    deque->tail = 0;
    return ooc_superDtor(ooc_arrayDequeClass(), self);
}

static void* ooc_arrayDequeClassInit(void) {
    if (ooc_classNew(&ArrayDequeClassInstance,
                     "ArrayDeque",
                     sizeof(OOC_ArrayDeque),
                     sizeof(OOC_ArrayDequeClass),
                     ooc_abstractCollectionClass(),
                     OOC_MODIFIER_NONE,
                     OOC_METHOD_CTOR, ooc_arrayDequeCtor,
                     OOC_METHOD_DTOR, ooc_arrayDequeDtor,
                     OOC_ABSTRACT_COLLECTION_METHOD_ITERATOR, ooc_arrayDequeGetIterator,
                     OOC_ABSTRACT_COLLECTION_METHOD_SIZE, ooc_arrayDequeSize,
                     OOC_ABSTRACT_COLLECTION_METHOD_ADD, ooc_arrayDequeAdd,
                     OOC_ARRAY_DEQUE_METHOD_OFFER, ooc_arrayDequeOffer,
                     OOC_ARRAY_DEQUE_METHOD_POLL, ooc_arrayDequePoll,
                     OOC_ARRAY_DEQUE_METHOD_PEEK, ooc_arrayDequePeek,
                     OOC_ARRAY_DEQUE_METHOD_ADD_FIRST, ooc_arrayDequeAddFirst,
                     OOC_ARRAY_DEQUE_METHOD_ADD_LAST, ooc_arrayDequeAddLast,
                     OOC_ARRAY_DEQUE_METHOD_REMOVE_FIRST, ooc_arrayDequeRemoveFirst,
                     OOC_ARRAY_DEQUE_METHOD_REMOVE_LAST, ooc_arrayDequeRemoveLast,
                     OOC_ARRAY_DEQUE_METHOD_GET_FIRST, ooc_arrayDequeGetFirst,
                     OOC_ARRAY_DEQUE_METHOD_GET_LAST, ooc_arrayDequeGetLast,
                     OOC_ARRAY_DEQUE_METHOD_PUSH, ooc_arrayDequePush,
                     OOC_ARRAY_DEQUE_METHOD_POP, ooc_arrayDequePop,
                     0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&ArrayDequeClassInstance);
        return NULL;
    }

    ArrayDequeInterfaces[0].interfaceClass = ooc_queueClass();
    ArrayDequeInterfaces[0].vtableOffset = offsetof(OOC_ArrayDequeClass, queueVtable);
    ArrayDequeInterfaces[1].interfaceClass = ooc_dequeClass();
    ArrayDequeInterfaces[1].vtableOffset = offsetof(OOC_ArrayDequeClass, dequeVtable);

    if (ooc_classSetInterface(&ArrayDequeClassInstance,
                              ArrayDequeInterfaces,
                              2) != OOC_ERROR_NONE) {
        ooc_classDestroy(&ArrayDequeClassInstance);
        return NULL;
    }

    return &ArrayDequeClassInstance;
}

void* ooc_arrayDequeClass(void) {
    if (!ArrayDequeClass) {
        ArrayDequeClass = ooc_arrayDequeClassInit();
    }
    return ArrayDequeClass;
}
