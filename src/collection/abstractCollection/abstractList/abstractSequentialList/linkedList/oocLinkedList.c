#include "oocLinkedList.h"
#include "oocAbstractIterator.h"
#include "oocCollection.h"

#include "oocError.h"
#include "oocLinkedList.r"
#include "oocAbstractSequentialList.h"
#include "oocDeque.h"
#include "oocList.h"
#include "oocObject.h"
#include "oocObject.r"
#include "oocAbstractIterator.r"
#include "oocAbstractListIterator.h"
#include "oocAbstractListIterator.r"
#include "oocQueue.h"

#include <stdlib.h>
#include <stddef.h>

typedef struct OOC_LinkedListIterator OOC_LinkedListIterator;
typedef struct OOC_LinkedListIteratorClass OOC_LinkedListIteratorClass;

struct OOC_LinkedListIterator {
    OOC_AbstractListIterator abstractListIterator;
    OOC_LinkedList* list;
    OOC_LinkedListNode* next;
    OOC_LinkedListNode* lastReturned;
    size_t nextIndex;
};

struct OOC_LinkedListIteratorClass {
    OOC_AbstractListIteratorClass class;
};

static OOC_LinkedListClass* LinkedListClass;
static OOC_LinkedListClass LinkedListClassInstance;

static OOC_LinkedListIteratorClass* LinkedListIteratorClass;
static OOC_LinkedListIteratorClass LinkedListIteratorClassInstance;

static void* ooc_linkedListIteratorClass(void);

static OOC_InterfaceImpl LinkedListInterfaces[2];

static OOC_Error ooc_linkedListNodeDestroy(OOC_LinkedListNode* node) {
    if (!node) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    ooc_destroy(node->data);
    free(node);
    return OOC_ERROR_NONE;
}

static OOC_LinkedListNode* ooc_linkedListCreateNode(void* element) {
    OOC_LinkedListNode* node = calloc(1, sizeof(OOC_LinkedListNode));
    if (!node) {
        return NULL;
    }
    node->data = element;
    return node;
}

static OOC_Error ooc_linkedListLinkFirst(OOC_LinkedList* list, OOC_LinkedListNode* node) {
    if (!list || !node) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    node->prev = NULL;
    node->next = list->head;
    list->head = node;
    if (node->next) {
        node->next->prev = node;
    } else {
        list->tail = node;
    }
    list->size++;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedListLinkLast(OOC_LinkedList* list, OOC_LinkedListNode* node) {
    if (!list || !node) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    node->next = NULL;
    node->prev = list->tail;
    list->tail = node;
    if (node->prev) {
        node->prev->next = node;
    } else {
        list->head = node;
    }
    list->size++;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedListLinkBefore(OOC_LinkedList* list,
                                          OOC_LinkedListNode* succ,
                                          OOC_LinkedListNode* node) {
    if (!list || !succ || !node) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    node->next = succ;
    node->prev = succ->prev;
    if (succ->prev) {
        succ->prev->next = node;
    } else {
        list->head = node;
    }
    succ->prev = node;
    list->size++;
    return OOC_ERROR_NONE;
}

static void* ooc_linkedListUnlinkFirst(OOC_LinkedList* list) {
    if (!list || !list->head) {
        return NULL;
    }
    OOC_LinkedListNode* firstNode = list->head;
    list->head = firstNode->next;
    if (list->head) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }
    firstNode->next = NULL;
    firstNode->prev = NULL;
    list->size--;
    return firstNode;
}

static void* ooc_linkedListUnlinkLast(OOC_LinkedList* list) {
    if (!list || !list->tail) {
        return NULL;
    }
    OOC_LinkedListNode* lastNode = list->tail;
    list->tail = lastNode->prev;
    if (list->tail) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }
    lastNode->next = NULL;
    lastNode->prev = NULL;
    list->size--;
    return lastNode;
}

static OOC_Error ooc_linkedListUnlink(OOC_LinkedList* list, OOC_LinkedListNode* node) {
    if (!list || !node) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    node->next = NULL;
    node->prev = NULL;
    if (list->size) {
        list->size--;
    }
    return OOC_ERROR_NONE;
}

static OOC_LinkedListNode* ooc_linkedListNodeAt(OOC_LinkedList* list, size_t index) {
    if (!list || index >= list->size) {
        return NULL;
    }
    OOC_LinkedListNode* node = list->head;
    for (size_t i = 0; i < index && node; i++) {
        node = node->next;
    }
    return node;
}

static bool ooc_linkedListIteratorHasNext(void* self) {
    if (!self) {
        return false;
    }
    OOC_LinkedListIterator* iterator = self;
    return iterator->nextIndex < iterator->list->size;
}

static void* ooc_linkedListIteratorNext(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_LinkedListIterator* iterator = self;
    if (!ooc_linkedListIteratorHasNext(iterator)) {
        return NULL;
    }
    ooc_abstractIteratorNext(iterator);
    iterator->lastReturned = iterator->next;
    iterator->next = iterator->next->next;
    iterator->nextIndex++;
    return iterator->lastReturned->data;
}

static OOC_Error ooc_linkedListIteratorRemove(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_LinkedListIterator* iterator = self;
    OOC_Error error = ooc_abstractIteratorRemove(iterator);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    if (!iterator->lastReturned) {
        return OOC_ERROR_INVALID_STATE;
    }
    // Determine if last operation was next() or previous()
    // After next(): lastReturned != next
    // After previous(): lastReturned == next
    if (iterator->lastReturned == iterator->next) {
        // Last operation was previous()
        // next points to the node being removed, so update it
        iterator->next = iterator->lastReturned->next;
    } else {
        // Last operation was next()
        // Decrement nextIndex as elements shift down
        iterator->nextIndex--;
    }
    error = ooc_linkedListUnlink(iterator->list, iterator->lastReturned);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    ooc_linkedListNodeDestroy(iterator->lastReturned);
    iterator->lastReturned = NULL;
    return OOC_ERROR_NONE;
}

static bool ooc_linkedListIteratorHasPrevious(void* self) {
    if (!self) {
        return false;
    }
    OOC_LinkedListIterator* iterator = self;
    return iterator->nextIndex > 0;
}

static void* ooc_linkedListIteratorPrevious(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_LinkedListIterator* iterator = self;
    if (!ooc_linkedListIteratorHasPrevious(iterator)) {
        return NULL;
    }
    ooc_abstractListIteratorPrevious(iterator);
    // If at the end (next is NULL), move to the tail
    if (iterator->next == NULL) {
        iterator->lastReturned = iterator->list->tail;
        iterator->next = iterator->list->tail;
    } else {
        // Move backwards to the previous node
        iterator->lastReturned = iterator->next->prev;
        iterator->next = iterator->next->prev;
    }
    iterator->nextIndex--;
    return iterator->lastReturned ? 
           iterator->lastReturned->data : 
           NULL;
}

static int ooc_linkedListIteratorNextIndex(void* self) {
    if (!self) {
        return -1;
    }
    OOC_LinkedListIterator* iterator = self;
    return (int)iterator->nextIndex;
}

static int ooc_linkedListIteratorPreviousIndex(void* self) {
    if (!self) {
        return -1;
    }
    OOC_LinkedListIterator* iterator = self;
    return (iterator->nextIndex == 0) ? -1 : (int)(iterator->nextIndex - 1);
}

static OOC_Error ooc_linkedListIteratorSet(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_LinkedListIterator* iterator = self;
    OOC_Error error = ooc_abstractListIteratorSet(self, element);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    if (!iterator->lastReturned) {
        return OOC_ERROR_INVALID_STATE;
    }
    if (iterator->lastReturned->data != element) {
        ooc_destroy(iterator->lastReturned->data);
        iterator->lastReturned->data = element;
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedListIteratorAdd(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_LinkedListIterator* iterator = self;
    OOC_Error error = ooc_abstractListIteratorAdd(self, element);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    OOC_LinkedListNode* node = ooc_linkedListCreateNode(element);
    if (!node) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    if (iterator->next == NULL) {
        error = ooc_linkedListLinkLast(iterator->list, node);
    } else {
        error = ooc_linkedListLinkBefore(iterator->list, iterator->next, node);
    }
    if (error != OOC_ERROR_NONE) {
        free(node);
        return error;
    }
    iterator->nextIndex++;
    iterator->lastReturned = NULL;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedListIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_LinkedListIterator* iterator = self;
    OOC_Error error = ooc_superCtor(ooc_linkedListIteratorClass(), iterator, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    iterator->list = va_arg(*args, OOC_LinkedList*);
    if (!iterator->list) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(iterator->list, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);
    iterator->nextIndex = va_arg(*args, size_t);
    if (iterator->nextIndex > iterator->list->size) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    iterator->lastReturned = NULL;
    iterator->next = ooc_linkedListNodeAt(iterator->list, iterator->nextIndex);
    return OOC_ERROR_NONE;
}

static void* ooc_linkedListIteratorClassInit(void) {
    if (ooc_classNew(&LinkedListIteratorClassInstance,
                     "LinkedListIterator",
                     sizeof(OOC_LinkedListIterator),
                     sizeof(OOC_LinkedListIteratorClass),
                     ooc_abstractListIteratorClass(),
                     OOC_MODIFIER_NONE,
                     OOC_METHOD_CTOR, ooc_linkedListIteratorCtor,
                     OOC_ABSTRACT_ITERATOR_METHOD_HAS_NEXT, ooc_linkedListIteratorHasNext,
                     OOC_ABSTRACT_ITERATOR_METHOD_NEXT, ooc_linkedListIteratorNext,
                     OOC_ABSTRACT_ITERATOR_METHOD_REMOVE, ooc_linkedListIteratorRemove,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_HAS_PREVIOUS, ooc_linkedListIteratorHasPrevious,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_PREVIOUS, ooc_linkedListIteratorPrevious,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_NEXT_INDEX, ooc_linkedListIteratorNextIndex,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_PREVIOUS_INDEX, ooc_linkedListIteratorPreviousIndex,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_SET, ooc_linkedListIteratorSet,
                     OOC_ABSTRACT_LIST_ITERATOR_METHOD_ADD, ooc_linkedListIteratorAdd,
                     0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&LinkedListIteratorClassInstance);
        return NULL;
    }

    return &LinkedListIteratorClassInstance;
}

static void* ooc_linkedListIteratorClass(void) {
    if (!LinkedListIteratorClass) {
        LinkedListIteratorClass = ooc_linkedListIteratorClassInit();
    }
    return LinkedListIteratorClass;
}

void* ooc_linkedListGetIterator(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);
    return ooc_new(ooc_linkedListIteratorClass(), self, (size_t)0, 0);
}

size_t ooc_linkedListSize(void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), 0);
    OOC_LinkedList* list = self;
    return list->size;
}

bool ooc_linkedListIsEmpty(void* self) {
    return ooc_collectionIsEmpty(self);
}

bool ooc_linkedListContains(void* self, void* element) {
    return ooc_collectionContains(self, element);
}

bool ooc_linkedListContainsAll(void* self, void* other) {
    return ooc_collectionContainsAll(self, other);
}

OOC_Error ooc_linkedListAdd(void* self, void* element) {
    return ooc_collectionAdd(self, element);
}

OOC_Error ooc_linkedListRemove(void* self, void* element) {
    return ooc_collectionRemove(self, element);
}

OOC_Error ooc_linkedListClear(void* self) {
    return ooc_collectionClear(self);
}

void* ooc_linkedListGetAt(void* self, size_t index) {
    return ooc_listGetAt(self, index);
}

OOC_Error ooc_linkedListSetAt(void* self, size_t index, void* element) {
    return ooc_listSetAt(self, index, element);
}

OOC_Error ooc_linkedListInsertAt(void* self, size_t index, void* element) {
    return ooc_listInsertAt(self, index, element);
}

OOC_Error ooc_linkedListRemoveAt(void* self, size_t index) {
    return ooc_listRemoveAt(self, index);
}

int ooc_linkedListIndexOf(void* self, void* element) {
    return ooc_listIndexOf(self, element);
}

int ooc_linkedListLastIndexOf(void* self, void* element) {
    return ooc_listLastIndexOf(self, element);
}

void* ooc_linkedListGetListIterator(void* self) {
    return ooc_listGetListIterator(self);
}

void* ooc_linkedListGetListIteratorAt(void* self, size_t index) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);
    OOC_LinkedList* list = (OOC_LinkedList*)self;
    if (index > list->size) {
        return NULL;
    }
    return ooc_new(ooc_linkedListIteratorClass(), list, index, 0);
}

OOC_Error ooc_linkedListAddFirst(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedList* list = self;
    OOC_LinkedListNode* node = ooc_linkedListCreateNode(element);
    if (!node) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    OOC_Error error = ooc_linkedListLinkFirst(list, node);
    if (error != OOC_ERROR_NONE) {
        free(node);
        return error;
    }
    return OOC_ERROR_NONE;
}

OOC_Error ooc_linkedListAddLast(void* self, void* element) {
    return ooc_linkedListAdd(self, element);
}

void* ooc_linkedListGetFirst(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);
    OOC_LinkedList* list = self;
    return list->head ? list->head->data : NULL;
}

void* ooc_linkedListGetLast(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);
    OOC_LinkedList* list = self;
    return list->tail ? list->tail->data : NULL;
}

void* ooc_linkedListRemoveFirst(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);
    OOC_LinkedList* list = self;
    OOC_LinkedListNode* node = ooc_linkedListUnlinkFirst(list);
    void* data = NULL;
    if (node) {
        data = node->data;
        free(node);
    }
    return data;
}

void* ooc_linkedListRemoveLast(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);
    OOC_LinkedList* list = self;
    OOC_LinkedListNode* node = ooc_linkedListUnlinkLast(list);
    void* data = NULL;
    if (node) {
        data = node->data;
        free(node);
    }
    return data;
}

OOC_Error ooc_linkedListPush(void* self, void* element) {
    return ooc_linkedListAddFirst(self, element);
}

void* ooc_linkedListPop(void* self) {
    return ooc_linkedListRemoveFirst(self);
}

OOC_Error ooc_linkedListOffer(void* self, void* element) {
    return ooc_linkedListAddLast(self, element);
}

void* ooc_linkedListPoll(void* self) {
    return ooc_linkedListRemoveFirst(self);
}

void* ooc_linkedListPeek(void* self) {
    return ooc_linkedListGetFirst(self);
}

static OOC_Error ooc_linkedListCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedList* list = self;
    OOC_Error error = ooc_superCtor(ooc_linkedListClass(), list, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedListDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_collectionClear(self);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    OOC_LinkedList* list = self;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return ooc_superDtor(ooc_linkedListClass(), self);
}

static void* ooc_linkedListClassInit(void) {
    if (ooc_classNew(&LinkedListClassInstance,
                     "LinkedList",
                     sizeof(OOC_LinkedList),
                     sizeof(OOC_LinkedListClass),
                     ooc_abstractSequentialListClass(),
                     OOC_MODIFIER_NONE,
                     OOC_METHOD_CTOR, ooc_linkedListCtor,
                     OOC_METHOD_DTOR, ooc_linkedListDtor,
                     OOC_ABSTRACT_COLLECTION_METHOD_ITERATOR, ooc_linkedListGetIterator,
                     OOC_ABSTRACT_COLLECTION_METHOD_SIZE, ooc_linkedListSize,
                     OOC_ABSTRACT_LIST_METHOD_GET_LIST_ITERATOR_AT, ooc_linkedListGetListIteratorAt,
                     OOC_LINKED_LIST_METHOD_OFFER, ooc_linkedListOffer,
                     OOC_LINKED_LIST_METHOD_POLL, ooc_linkedListPoll,
                     OOC_LINKED_LIST_METHOD_PEEK, ooc_linkedListPeek,
                     OOC_LINKED_LIST_METHOD_ADD_FIRST, ooc_linkedListAddFirst,
                     OOC_LINKED_LIST_METHOD_ADD_LAST, ooc_linkedListAddLast,
                     OOC_LINKED_LIST_METHOD_REMOVE_FIRST, ooc_linkedListRemoveFirst,
                     OOC_LINKED_LIST_METHOD_REMOVE_LAST, ooc_linkedListRemoveLast,
                     OOC_LINKED_LIST_METHOD_GET_FIRST, ooc_linkedListGetFirst,
                     OOC_LINKED_LIST_METHOD_GET_LAST, ooc_linkedListGetLast,
                     OOC_LINKED_LIST_METHOD_PUSH, ooc_linkedListPush,
                     OOC_LINKED_LIST_METHOD_POP, ooc_linkedListPop,
                     0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&LinkedListClassInstance);
        return NULL;
    }

    LinkedListInterfaces[0].interfaceClass = ooc_queueClass();
    LinkedListInterfaces[0].vtableOffset = offsetof(OOC_LinkedListClass, queueVtable);
    LinkedListInterfaces[1].interfaceClass = ooc_dequeClass();
    LinkedListInterfaces[1].vtableOffset = offsetof(OOC_LinkedListClass, dequeVtable);

    if (ooc_classSetInterface(&LinkedListClassInstance,
                              LinkedListInterfaces,
                                     2) != OOC_ERROR_NONE) {
        ooc_classDestroy(&LinkedListClassInstance);
        return NULL;
    }

    return &LinkedListClassInstance;
}

void* ooc_linkedListClass(void) {
    if (!LinkedListClass) {
        LinkedListClass = ooc_linkedListClassInit();
    }
    return LinkedListClass;
}
