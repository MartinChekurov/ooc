#include "oocArrayDeque.h"
#include "oocArrayDeque.r"
#include "oocDeque.h"
#include "oocDeque.r"
#include "oocQueue.h"
#include "oocQueue.r"
#include "oocCollection.r"
#include "oocIterable.r"
#include "oocIterator.h"
#include "oocIterator.r"
#include "oocObject.h"
#include "oocBaseIterator.h"
#include "oocBaseIterator.r"
#include "oocStringBuffer.h"
#include "oocAbstractCollection.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CAPACITY 16

typedef struct OOC_ArrayDequeIterator OOC_ArrayDequeIterator;
typedef struct OOC_ArrayDequeIteratorClass OOC_ArrayDequeIteratorClass;

struct OOC_ArrayDequeIterator {
    OOC_BaseIterator object;
    OOC_ArrayDeque* deque;
    size_t currentIndex;      // Current position in circular buffer
    size_t remaining;         // Number of elements remaining to iterate
    size_t lastReturnedIndex; // Index of last element returned by next()
    bool canRemove;           // Whether remove() can be called
};

struct OOC_ArrayDequeIteratorClass {
    OOC_BaseIteratorClass class;
};

static OOC_ArrayDequeClass* ArrayDequeClass;
static OOC_ArrayDequeClass ArrayDequeClassInstance;

static OOC_ArrayDequeIteratorClass* ArrayDequeIteratorClass;
static OOC_ArrayDequeIteratorClass ArrayDequeIteratorClassInstance;

static OOC_Error resize(OOC_ArrayDeque* deque, size_t newCapacity) {
    if (!deque || newCapacity < deque->size) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    void** newElements = malloc(newCapacity * sizeof(void*));
    if (!newElements) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    for (size_t i = 0; i < deque->size; i++) {
        newElements[i] = deque->elements[(deque->head + i) % deque->capacity];
    }
    free(deque->elements);
    deque->elements = newElements;
    deque->capacity = newCapacity;
    deque->head = 0;
    deque->tail = deque->size;
    return OOC_ERROR_NONE;
}

// Collection interface methods
static size_t ooc_arrayDequeSize(void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), 0);
    OOC_ArrayDeque* deque = self;
    return deque->size;
}

static bool ooc_arrayDequeIsEmpty(void* self) {
    if (!self) {
        return true;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), true);
    OOC_ArrayDeque* deque = self;
    return deque->size == 0;
}

static bool ooc_arrayDequeContains(void* self, void* element) {
    if (!self) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), false);
    OOC_ArrayDeque* deque = self;
    for (size_t i = 0; i < deque->size; i++) {
        size_t index = (deque->head + i) % deque->capacity;
        if (ooc_equals(deque->elements[index], element)) {
            return true;
        }
    }
    return false;
}

static OOC_Error ooc_arrayDequeAdd(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayDeque* deque = self;
    if (deque->size == deque->capacity) {
        OOC_Error error = resize(deque, deque->capacity * 2);
        if (error != OOC_ERROR_NONE) {
            return error;
        }
    }
    deque->elements[deque->tail] = element;
    deque->tail = (deque->tail + 1) % deque->capacity;
    deque->size++;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_arrayDequeRemove(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayDeque* deque = self;
    for (size_t i = 0; i < deque->size; i++) {
        size_t index = (deque->head + i) % deque->capacity;
        if (ooc_equals(deque->elements[index], element)) {
            ooc_destroy(deque->elements[index]);
            // Shift elements to fill the gap
            for (size_t j = i; j < deque->size - 1; j++) {
                size_t currentIndex = (deque->head + j) % deque->capacity;
                size_t nextIndex = (deque->head + j + 1) % deque->capacity;
                deque->elements[currentIndex] = deque->elements[nextIndex];
            }
            deque->tail = (deque->tail - 1 + deque->capacity) % deque->capacity;
            deque->size--;
            return OOC_ERROR_NONE;
        }
    }
    return OOC_ERROR_NOT_FOUND;
}

static OOC_Error ooc_arrayDequeClear(void* self) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayDeque* deque = self;
    for (size_t i = 0; i < deque->size; i++) {
        size_t index = (deque->head + i) % deque->capacity;
        ooc_destroy(deque->elements[index]);
        deque->elements[index] = NULL;
    }
    deque->size = 0;
    deque->head = 0;
    deque->tail = 0;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_arrayDequeAddFirst(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayDeque* deque = self;
    if (deque->size == deque->capacity) {
        OOC_Error error = resize(deque, deque->capacity * 2);
        if (error != OOC_ERROR_NONE) {
            return error;
        }
    }
    deque->head = (deque->head - 1 + deque->capacity) % deque->capacity;
    deque->elements[deque->head] = element;
    deque->size++;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_arrayDequeAddLast(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_arrayDequeAdd(self, element);
}

static void* ooc_arrayDequeGetFirst(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), NULL);
    OOC_ArrayDeque* deque = self;
    if (deque->size == 0) {
        return NULL;
    }
    return deque->elements[deque->head];
}

static void* ooc_arrayDequeGetLast(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), NULL);
    OOC_ArrayDeque* deque = self;
    if (deque->size == 0) {
        return NULL;
    }
    size_t lastIndex = (deque->tail - 1 + deque->capacity) % deque->capacity;
    return deque->elements[lastIndex];
}

static void* ooc_arrayDequeRemoveFirst(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), NULL);
    OOC_ArrayDeque* deque = self;
    if (deque->size == 0) {
        return NULL;
    }
    void* element = deque->elements[deque->head];
    deque->head = (deque->head + 1) % deque->capacity;
    deque->size--;
    return element;
}

static void* ooc_arrayDequeRemoveLast(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), NULL);
    OOC_ArrayDeque* deque = self;
    if (deque->size == 0) {
        return NULL;
    }
    deque->tail = (deque->tail - 1 + deque->capacity) % deque->capacity;
    void* element = deque->elements[deque->tail];
    deque->size--;
    return element;
}

static void* ooc_arrayDequePeek(void* self) {
    return ooc_arrayDequeGetFirst(self);
}

static bool ooc_arrayDequeIteratorHasNext(void* self) {
    if (!self) {
        return false;
    }
    OOC_TYPE_CHECK(self, ArrayDequeIteratorClass, false);
    OOC_ArrayDequeIterator* iterator = self;
    return iterator->remaining > 0;
}

static void* ooc_arrayDequeIteratorNext(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ArrayDequeIteratorClass, NULL);
    OOC_ArrayDequeIterator* iterator = self;
    ooc_superBaseIteratorNext(iterator);
    if (iterator->remaining == 0) {
        return NULL;
    }
    iterator->lastReturnedIndex = iterator->currentIndex;
    iterator->canRemove = true;
    void* element = iterator->deque->elements[iterator->currentIndex];
    iterator->currentIndex = (iterator->currentIndex + 1) % iterator->deque->capacity;
    iterator->remaining--;
    return element;
}

static OOC_Error ooc_arrayDequeIteratorRemove(void* self) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ArrayDequeIteratorClass, OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayDequeIterator* iterator = self;
    ooc_superBaseIteratorRemove(iterator);
    if (!iterator->canRemove) {
        return OOC_ERROR_INVALID_STATE;
    }
    OOC_ArrayDeque* deque = iterator->deque;
    ooc_destroy(deque->elements[iterator->lastReturnedIndex]);
    size_t relativePos;
    if (iterator->lastReturnedIndex >= deque->head) {
        relativePos = iterator->lastReturnedIndex - deque->head;
    } else {
        relativePos = (deque->capacity - deque->head) + iterator->lastReturnedIndex;
    }
    for (size_t i = relativePos; i < deque->size - 1; i++) {
        size_t currentIdx = (deque->head + i) % deque->capacity;
        size_t nextIdx = (deque->head + i + 1) % deque->capacity;
        deque->elements[currentIdx] = deque->elements[nextIdx];
    }
    deque->tail = (deque->tail - 1 + deque->capacity) % deque->capacity;
    deque->size--;
    iterator->currentIndex = (iterator->currentIndex - 1 + deque->capacity) % deque->capacity;
    iterator->canRemove = false;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_arrayDequeIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ArrayDequeIteratorClass, OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayDequeIterator* iterator = self;
    OOC_Error error = ooc_superCtor(iterator, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    void* iteratorInterface = ooc_new(ooc_iteratorClass(),
                                      OOC_ITERATOR_METHOD_HAS_NEXT, ooc_arrayDequeIteratorHasNext,
                                      OOC_ITERATOR_METHOD_NEXT, ooc_arrayDequeIteratorNext,
                                      OOC_ITERATOR_METHOD_REMOVE, ooc_arrayDequeIteratorRemove,
                                      0);
    error = ooc_addInterface(iterator, iteratorInterface);
    if (error != OOC_ERROR_NONE) {
        ooc_destroy(iteratorInterface);
        return error;
    }
    iterator->deque = va_arg(*args, OOC_ArrayDeque*);
    if (!iterator->deque) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    iterator->currentIndex = iterator->deque->head;
    iterator->remaining = iterator->deque->size;
    iterator->lastReturnedIndex = 0;
    iterator->canRemove = false;
    return OOC_ERROR_NONE;
}

static void* ooc_arrayDequeIteratorClassInit(void) {
    if (ooc_classNew(&ArrayDequeIteratorClassInstance,
                    "ArrayDequeIterator",
                    sizeof(OOC_ArrayDequeIterator),
                    sizeof(OOC_ArrayDequeIteratorClass),
                    ooc_baseIteratorClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_arrayDequeIteratorCtor,
                    0) != OOC_ERROR_NONE) {
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

static void* ooc_arrayDequeGetIterator(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), NULL);
    OOC_ArrayDeque* deque = self;
    return ooc_new(ooc_arrayDequeIteratorClass(), deque, 0);
}

static OOC_Error ooc_arrayDequeCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayDeque* deque = self;
    OOC_Error error = ooc_superCtor(deque, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    void* interface = ooc_new(ooc_dequeClass(),
                          OOC_ITERABLE_METHOD_ITERATOR, ooc_arrayDequeGetIterator,
                          OOC_COLLECTION_METHOD_SIZE, ooc_arrayDequeSize,
                          OOC_COLLECTION_METHOD_IS_EMPTY, ooc_arrayDequeIsEmpty,
                          OOC_COLLECTION_METHOD_CONTAINS, ooc_arrayDequeContains,
                          OOC_COLLECTION_METHOD_ADD, ooc_arrayDequeAdd,
                          OOC_COLLECTION_METHOD_REMOVE, ooc_arrayDequeRemove,
                          OOC_COLLECTION_METHOD_CLEAR, ooc_arrayDequeClear,
                          OOC_QUEUE_METHOD_PUSH, ooc_arrayDequeAdd,
                          OOC_QUEUE_METHOD_POP, ooc_arrayDequeRemoveFirst,
                          OOC_QUEUE_METHOD_PEEK, ooc_arrayDequePeek,
                          OOC_DEQUE_METHOD_ADD_FIRST, ooc_arrayDequeAddFirst,
                          OOC_DEQUE_METHOD_ADD_LAST, ooc_arrayDequeAddLast,
                          OOC_DEQUE_METHOD_REMOVE_FIRST, ooc_arrayDequeRemoveFirst,
                          OOC_DEQUE_METHOD_REMOVE_LAST, ooc_arrayDequeRemoveLast,
                          OOC_DEQUE_METHOD_GET_FIRST, ooc_arrayDequeGetFirst,
                          OOC_DEQUE_METHOD_GET_LAST, ooc_arrayDequeGetLast,
                          0);
    error = ooc_addInterface(deque, interface);
    if (error != OOC_ERROR_NONE) {
        ooc_destroy(interface);
        return error;
    }
    deque->capacity = va_arg(*args, size_t);
    if (!deque->capacity) {
        deque->capacity = DEFAULT_CAPACITY;
    }
    deque->size = 0;
    deque->head = 0;
    deque->tail = 0;
    deque->elements = malloc(deque->capacity * sizeof(void*));
    if (!deque->elements) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_arrayDequeDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_arrayDequeClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_ArrayDeque* deque = self;
    OOC_Error error = ooc_arrayDequeClear(self);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    free(deque->elements);
    deque->elements = NULL;
    deque->size = 0;
    deque->capacity = 0;
    deque->head = 0;
    deque->tail = 0;
    return ooc_superDtor(deque);
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
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &ArrayDequeClassInstance;
}

void* ooc_arrayDequeClass() {
    if (!ArrayDequeClass) {
        ArrayDequeClass = ooc_arrayDequeClassInit();
    }
    return ArrayDequeClass;
}
