#ifndef OOC_ARRAY_DEQUE_R_
#define OOC_ARRAY_DEQUE_R_

#include "oocAbstractCollection.r"
#include "oocError.h"
#include <stddef.h>

typedef struct OOC_ArrayDeque OOC_ArrayDeque;
typedef struct OOC_ArrayDequeClass OOC_ArrayDequeClass;

struct OOC_ArrayDeque {
    OOC_AbstractCollection abstractCollection;
    void** elements;    // Circular buffer
    size_t capacity;    // Allocated capacity
    size_t size;        // Number of elements
    size_t head;        // Index of first element
    size_t tail;        // Index where next element will be inserted
};

struct OOC_ArrayDequeClass {
    OOC_AbstractCollectionClass class;
};

#endif
