#ifndef OOC_ARRAY_DEQUE_R_
#define OOC_ARRAY_DEQUE_R_

#include "oocAbstractCollection.r"
#include "oocDeque.r"
#include <stddef.h>

typedef struct OOC_ArrayDeque OOC_ArrayDeque;
typedef struct OOC_ArrayDequeClass OOC_ArrayDequeClass;

struct OOC_ArrayDeque {
    OOC_AbstractCollection abstractCollection;
    void** elements;
    size_t capacity;
    size_t size;
    size_t head;
    size_t tail;
};

struct OOC_ArrayDequeClass {
    OOC_AbstractCollectionClass class;
    OOC_DequeVtable dequeVtable;
};

#define OOC_ARRAY_DEQUE_METHOD_PUSH         offsetof(OOC_ArrayDequeClass, dequeVtable.queueVtable.push)
#define OOC_ARRAY_DEQUE_METHOD_POP          offsetof(OOC_ArrayDequeClass, dequeVtable.queueVtable.pop)
#define OOC_ARRAY_DEQUE_METHOD_PEEK         offsetof(OOC_ArrayDequeClass, dequeVtable.queueVtable.peek)
#define OOC_ARRAY_DEQUE_METHOD_ADD_FIRST    offsetof(OOC_ArrayDequeClass, dequeVtable.addFirst)
#define OOC_ARRAY_DEQUE_METHOD_ADD_LAST     offsetof(OOC_ArrayDequeClass, dequeVtable.addLast)
#define OOC_ARRAY_DEQUE_METHOD_REMOVE_FIRST offsetof(OOC_ArrayDequeClass, dequeVtable.removeFirst)
#define OOC_ARRAY_DEQUE_METHOD_REMOVE_LAST  offsetof(OOC_ArrayDequeClass, dequeVtable.removeLast)
#define OOC_ARRAY_DEQUE_METHOD_GET_FIRST    offsetof(OOC_ArrayDequeClass, dequeVtable.getFirst)
#define OOC_ARRAY_DEQUE_METHOD_GET_LAST     offsetof(OOC_ArrayDequeClass, dequeVtable.getLast)

#endif /* OOC_ARRAY_DEQUE_R_ */
