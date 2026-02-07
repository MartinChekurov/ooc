#ifndef OOC_LINKED_LIST_R_
#define OOC_LINKED_LIST_R_

#include "oocAbstractCollection.r"
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
    OOC_AbstractCollection abstractCollection;
    OOC_LinkedListNode* head;
    OOC_LinkedListNode* tail;
    size_t size;
};

struct OOC_LinkedListClass {
    OOC_AbstractCollectionClass class;
};

#endif
