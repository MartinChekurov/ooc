#ifndef OOC_LINKED_LIST_R_
#define OOC_LINKED_LIST_R_

#include "oocAbstractSequentialList.r"
#include "oocDeque.r"
#include "oocQueue.r"
#include "oocError.h"
#include <stddef.h>

typedef struct OOC_LinkedListNode OOC_LinkedListNode;
typedef struct OOC_LinkedList OOC_LinkedList;
typedef struct OOC_LinkedListClass OOC_LinkedListClass;

struct OOC_LinkedListNode {
    void* data;
    struct OOC_LinkedListNode* next;
    struct OOC_LinkedListNode* prev;
};

struct OOC_LinkedList {
    OOC_AbstractSequentialList abstractSequentialList;
    OOC_LinkedListNode* head;
    OOC_LinkedListNode* tail;
    size_t size;
};

struct OOC_LinkedListClass {
    OOC_AbstractSequentialListClass class;
    OOC_QueueVtable queueVtable;
    OOC_DequeVtable dequeVtable;
};

#define OOC_LINKED_LIST_METHOD_OFFER        offsetof(OOC_LinkedListClass, queueVtable.offer)
#define OOC_LINKED_LIST_METHOD_POLL         offsetof(OOC_LinkedListClass, queueVtable.poll)
#define OOC_LINKED_LIST_METHOD_PEEK         offsetof(OOC_LinkedListClass, queueVtable.peek)
#define OOC_LINKED_LIST_METHOD_ADD_FIRST    offsetof(OOC_LinkedListClass, dequeVtable.addFirst)
#define OOC_LINKED_LIST_METHOD_ADD_LAST     offsetof(OOC_LinkedListClass, dequeVtable.addLast)
#define OOC_LINKED_LIST_METHOD_REMOVE_FIRST offsetof(OOC_LinkedListClass, dequeVtable.removeFirst)
#define OOC_LINKED_LIST_METHOD_REMOVE_LAST  offsetof(OOC_LinkedListClass, dequeVtable.removeLast)
#define OOC_LINKED_LIST_METHOD_GET_FIRST    offsetof(OOC_LinkedListClass, dequeVtable.getFirst)
#define OOC_LINKED_LIST_METHOD_GET_LAST     offsetof(OOC_LinkedListClass, dequeVtable.getLast)
#define OOC_LINKED_LIST_METHOD_PUSH         offsetof(OOC_LinkedListClass, dequeVtable.push)
#define OOC_LINKED_LIST_METHOD_POP          offsetof(OOC_LinkedListClass, dequeVtable.pop)

#endif /* OOC_LINKED_LIST_R_ */
