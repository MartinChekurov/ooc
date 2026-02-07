#include "oocLinkedList.h"
#include "oocLinkedList.r"
#include "oocList.h"
#include "oocList.r"
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
#include "oocAbstractCollection.h"
#include <stdlib.h>
#include <string.h>

typedef struct OOC_LinkedListIterator OOC_LinkedListIterator;
typedef struct OOC_LinkedListIteratorClass OOC_LinkedListIteratorClass;

struct OOC_LinkedListIterator {
    OOC_BaseIterator object;
    OOC_LinkedList* list;
    OOC_LinkedListNode* currentNode;
    OOC_LinkedListNode* lastReturned;
};

struct OOC_LinkedListIteratorClass {
    OOC_BaseIteratorClass class;
};

static OOC_LinkedListClass* LinkedListClass;
static OOC_LinkedListClass LinkedListClassInstance;

static OOC_LinkedListIteratorClass* LinkedListIteratorClass;
static OOC_LinkedListIteratorClass LinkedListIteratorClassInstance;

static OOC_Error destroyNode(OOC_LinkedListNode* node) {
    if (!node) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    ooc_destroy(node->data);
    free(node);
    return OOC_ERROR_NONE;
}

static OOC_LinkedListNode* createNode(void* data) {
    OOC_LinkedListNode* node = malloc(sizeof(OOC_LinkedListNode));
    if (!node) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

static OOC_LinkedListNode* getNodeAt(OOC_LinkedList* list, size_t index) {
    if (!list) {
        return NULL;
    }
    if (index >= list->size) {
        return NULL;
    }
    OOC_LinkedListNode* current;
    // Optimize: start from head or tail depending on which is closer
    if (index < list->size / 2) {
        current = list->head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
    } else {
        current = list->tail;
        for (size_t i = list->size - 1; i > index; i--) {
            current = current->prev;
        }
    }
    return current;
}

static size_t ooc_linkedListSize(void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), 0);
    OOC_LinkedList* list = self;
    return list->size;
}

static bool ooc_linkedListIsEmpty(void* self) {
    if (!self) {
        return true;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), true);
    OOC_LinkedList* list = self;
    return list->size == 0;
}

static void* ooc_linkedListGet(void* self, void* element) {
    if (!self) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), false);
    OOC_LinkedList* list = self;
    OOC_LinkedListNode* current = list->head;
    while (current) {
        if (ooc_equals(current->data, element)) {
            return current->data;
        }
        current = current->next;
    }
    return false;
}

static bool ooc_linkedListContains(void* self, void* element) {
    return ooc_linkedListGet(self, element) != NULL;
}

static OOC_Error ooc_linkedListAdd(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedList* list = self;
    OOC_LinkedListNode* node = createNode(element);
    if (!node) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    if (list->size == 0) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    list->size++;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedListRemove(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedList* list = self;
    OOC_LinkedListNode* current = list->head;
    while (current) {
        if (ooc_equals(current->data, element)) {
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                list->head = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                list->tail = current->prev;
            }
            destroyNode(current);
            list->size--;
            return OOC_ERROR_NONE;
        }
        current = current->next;
    }
    return OOC_ERROR_NOT_FOUND;
}

static OOC_Error ooc_linkedListClear(void* self) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedList* list = self;
    OOC_LinkedListNode* current = list->head;
    while (current) {
        OOC_LinkedListNode* next = current->next;
        destroyNode(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return OOC_ERROR_NONE;
}

static void* ooc_linkedListGetAt(void* self, size_t index) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);
    OOC_LinkedList* list = self;
    OOC_LinkedListNode* node = getNodeAt(list, index);
    return node ? node->data : NULL;
}

static OOC_Error ooc_linkedListSetAt(void* self, size_t index, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedList* list = self;
    OOC_LinkedListNode* node = getNodeAt(list, index);
    if (!node) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    node->data = element;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedListInsertAt(void* self, size_t index, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedList* list = self;
    if (index > list->size) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_LinkedListNode* newNode = createNode(element);
    if (!newNode) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    if (index == 0) {
        newNode->next = list->head;
        if (list->head) {
            list->head->prev = newNode;
        }
        list->head = newNode;
        if (list->size == 0) {
            list->tail = newNode;
        }
    } else if (index == list->size) {
        newNode->prev = list->tail;
        if (list->tail) {
            list->tail->next = newNode;
        }
        list->tail = newNode;
    } else {
        OOC_LinkedListNode* current = getNodeAt(list, index);
        newNode->prev = current->prev;
        newNode->next = current;
        if (current->prev) {
            current->prev->next = newNode;
        }
        current->prev = newNode;
    }
    list->size++;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedListRemoveAt(void* self, size_t index) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedList* list = self;
    OOC_LinkedListNode* node = getNodeAt(list, index);
    if (!node) {
        return OOC_ERROR_NOT_FOUND;
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
    destroyNode(node);
    list->size--;
    return OOC_ERROR_NONE;
}

static int ooc_linkedListIndexOf(void* self, void* element) {
    if (!self) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), -1);
    OOC_LinkedList* list = self;
    OOC_LinkedListNode* current = list->head;
    int index = 0;
    while (current) {
        if (ooc_equals(current->data, element)) {
            return index;
        }
        current = current->next;
        index++;
    }
    return -1;
}

static int ooc_linkedListLastIndexOf(void* self, void* element) {
    if (!self) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), -1);
    OOC_LinkedList* list = self;
    OOC_LinkedListNode* current = list->tail;
    int index = (int)list->size - 1;
    while (current) {
        if (ooc_equals(current->data, element)) {
            return index;
        }
        current = current->prev;
        index--;
    }
    return -1;
}

OOC_Error ooc_linkedListAddFirst(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_linkedListInsertAt(self, 0, element);
}

OOC_Error ooc_linkedListAddLast(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_linkedListAdd(self, element);
}

void* ooc_linkedListGetFirst(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);
    OOC_LinkedList* list = self;
    return list->size ? list->head->data : NULL;
}

void* ooc_linkedListGetLast(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);
    OOC_LinkedList* list = self;
    return list->size ? list->tail->data : NULL;
}

void* ooc_linkedListRemoveFirst(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);
    OOC_LinkedList* list = self;
    if (list->size == 0) {
        return NULL;
    }
    OOC_LinkedListNode* node = list->head;
    void* data = node->data;
    if (node->next) {
        node->next->prev = NULL;
        list->head = node->next;
    } else {
        // Only one node in the list
        list->head = NULL;
        list->tail = NULL;
    }
    free(node);
    list->size--;
    return data;
}

void* ooc_linkedListRemoveLast(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);
    OOC_LinkedList* list = self;
    if (list->size == 0) {
        return NULL;
    }
    OOC_LinkedListNode* node = list->tail;
    void* data = node->data;
    if (node->prev) {
        node->prev->next = NULL;
        list->tail = node->prev;
    } else {
        // Only one node in the list
        list->head = NULL;
        list->tail = NULL;
    }
    free(node);
    list->size--;
    return data;
}

static bool ooc_linkedListIteratorHasNext(void* self) {
    if (!self) {
        return false;
    }
    OOC_TYPE_CHECK(self, LinkedListIteratorClass, false);
    OOC_LinkedListIterator* iterator = self;
    return iterator->currentNode != NULL;
}

static void* ooc_linkedListIteratorNext(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, LinkedListIteratorClass, NULL);
    OOC_LinkedListIterator* iterator = self;
    ooc_superBaseIteratorNext(iterator);
    if (!iterator->currentNode) {
        return NULL;
    }
    iterator->lastReturned = iterator->currentNode;
    void* data = iterator->currentNode->data;
    iterator->currentNode = iterator->currentNode->next;
    return data;
}

static OOC_Error ooc_linkerListIteratorRemove(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, LinkedListIteratorClass, OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedListIterator* iterator = self;
    ooc_superBaseIteratorRemove(iterator);
    if (!iterator->lastReturned) {
        return OOC_ERROR_NOT_FOUND;
    }
    OOC_LinkedList* list = iterator->list;
    OOC_LinkedListNode* nodeToRemove = iterator->lastReturned;
    // Unlink
    if (nodeToRemove->prev) {
        nodeToRemove->prev->next = nodeToRemove->next;
    } else {
        list->head = nodeToRemove->next;
    }
    if (nodeToRemove->next) {
        nodeToRemove->next->prev = nodeToRemove->prev;
    } else {
        list->tail = nodeToRemove->prev;
    }
    destroyNode(nodeToRemove);
    list->size--;
    iterator->lastReturned = NULL;  // Prevent double removal
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedListIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, LinkedListIteratorClass, OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedListIterator* iterator = self;
    OOC_Error error = ooc_superCtor(iterator, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    void* iteratorInterface = ooc_new(ooc_iteratorClass(),
                                      OOC_ITERATOR_METHOD_HAS_NEXT, ooc_linkedListIteratorHasNext,
                                      OOC_ITERATOR_METHOD_NEXT, ooc_linkedListIteratorNext,
                                      OOC_ITERATOR_METHOD_REMOVE, ooc_linkerListIteratorRemove,
                                      0);
    error = ooc_addInterface(iterator, iteratorInterface);
    if (error != OOC_ERROR_NONE) {
        ooc_destroy(iteratorInterface);
        return error;
    }
    iterator->list = va_arg(*args, OOC_LinkedList*);
    if (!iterator->list) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    iterator->currentNode = iterator->list->head;
    iterator->lastReturned = NULL;
    return OOC_ERROR_NONE;
}

static void* ooc_linkedListIteratorClassInit(void) {
    if (ooc_classNew(&LinkedListIteratorClassInstance,
                    "LinkedListIterator",
                    sizeof(OOC_LinkedListIterator),
                    sizeof(OOC_LinkedListIteratorClass),
                    ooc_baseIteratorClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_linkedListIteratorCtor,
                    0) != OOC_ERROR_NONE) {
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
    OOC_LinkedList* list = self;
    return ooc_new(ooc_linkedListIteratorClass(), list, 0);
}

static OOC_Error ooc_linkedListCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedList* list = self;
    OOC_Error error = ooc_superCtor(list, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    void* interface = ooc_new(ooc_listClass(),
                                OOC_ITERABLE_METHOD_ITERATOR, ooc_linkedListGetIterator,
                                OOC_COLLECTION_METHOD_SIZE, ooc_linkedListSize,
                                OOC_COLLECTION_METHOD_IS_EMPTY, ooc_linkedListIsEmpty,
                                OOC_COLLECTION_METHOD_CONTAINS, ooc_linkedListContains,
                                OOC_COLLECTION_METHOD_ADD, ooc_linkedListAdd,
                                OOC_COLLECTION_METHOD_REMOVE, ooc_linkedListRemove,
                                OOC_COLLECTION_METHOD_CLEAR, ooc_linkedListClear,
                                OOC_LIST_METHOD_GET_AT, ooc_linkedListGetAt,
                                OOC_LIST_METHOD_SET_AT, ooc_linkedListSetAt,
                                OOC_LIST_METHOD_INSERT_AT, ooc_linkedListInsertAt,
                                OOC_LIST_METHOD_REMOVE_AT, ooc_linkedListRemoveAt,
                                OOC_LIST_METHOD_INDEX_OF, ooc_linkedListIndexOf,
                                OOC_LIST_METHOD_LAST_INDEX_OF, ooc_linkedListLastIndexOf,
                                OOC_QUEUE_METHOD_PUSH, ooc_linkedListAdd,
                                OOC_QUEUE_METHOD_POP, ooc_linkedListRemoveFirst,
                                OOC_QUEUE_METHOD_PEEK, ooc_linkedListGetFirst,
                                OOC_DEQUE_METHOD_ADD_FIRST, ooc_linkedListAddFirst,
                                OOC_DEQUE_METHOD_ADD_LAST, ooc_linkedListAddLast,
                                OOC_DEQUE_METHOD_REMOVE_FIRST, ooc_linkedListRemoveFirst,
                                OOC_DEQUE_METHOD_REMOVE_LAST, ooc_linkedListRemoveLast,
                                OOC_DEQUE_METHOD_GET_FIRST, ooc_linkedListGetFirst,
                                OOC_DEQUE_METHOD_GET_LAST, ooc_linkedListGetLast,
                                0);
    error = ooc_addInterface(list, interface);
    if (error != OOC_ERROR_NONE) {
        ooc_destroy(interface);
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
    OOC_Error error = ooc_linkedListClear(self);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    return ooc_superDtor(self);
}

static void* ooc_linkedListClassInit(void) {
    if (ooc_classNew(&LinkedListClassInstance,
                    "LinkedList",
                    sizeof(OOC_LinkedList),
                    sizeof(OOC_LinkedListClass),
                    ooc_abstractCollectionClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_linkedListCtor,
                    OOC_METHOD_DTOR, ooc_linkedListDtor,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &LinkedListClassInstance;
}

void* ooc_linkedListClass() {
    if (!LinkedListClass) {
        LinkedListClass = ooc_linkedListClassInit();
    }
    return LinkedListClass;
}