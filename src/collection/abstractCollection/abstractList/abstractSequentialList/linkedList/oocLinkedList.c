#include "oocLinkedList.h"
#include "oocCollection.h"
#include "oocLinkedList.r"
#include "oocAbstractSequentialList.h"
#include "oocDeque.h"
#include "oocList.h"
#include "oocListIterator.h"
#include "oocListIterator.r"
#include "oocObject.h"
#include "oocObject.r"
#include "oocBaseIterator.h"
#include "oocBaseIterator.r"

#include <stdlib.h>
#include <stddef.h>

typedef struct OOC_LinkedListIterator OOC_LinkedListIterator;
typedef struct OOC_LinkedListIteratorClass OOC_LinkedListIteratorClass;

struct OOC_LinkedListIterator {
    OOC_BaseIterator baseIterator;
    OOC_LinkedList* list;
    OOC_LinkedListNode* next;
    OOC_LinkedListNode* lastReturned;
    size_t nextIndex;
};

struct OOC_LinkedListIteratorClass {
    OOC_BaseIteratorClass class;
    OOC_ListIteratorVtable listIteratorVtable;
};

static OOC_LinkedListClass* LinkedListClass;
static OOC_LinkedListClass LinkedListClassInstance;

static OOC_LinkedListIteratorClass* LinkedListIteratorClass;
static OOC_LinkedListIteratorClass LinkedListIteratorClassInstance;

static OOC_InterfaceImpl LinkedListInterfaces[1];
static OOC_InterfaceImpl LinkedListIteratorInterfaces[1];

static OOC_LinkedListNode* ooc_linkedListCreateNode(void* element) {
    OOC_LinkedListNode* node = (OOC_LinkedListNode*)malloc(sizeof(OOC_LinkedListNode));
    if (!node) {
        return NULL;
    }
    node->data = element;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

static void ooc_linkedListLinkFirst(OOC_LinkedList* list, OOC_LinkedListNode* node) {
    node->prev = NULL;
    node->next = list->head;

    if (list->head) {
        list->head->prev = node;
    } else {
        list->tail = node;
    }

    list->head = node;
    list->size++;
}

static void ooc_linkedListLinkLast(OOC_LinkedList* list, OOC_LinkedListNode* node) {
    node->next = NULL;
    node->prev = list->tail;

    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }

    list->tail = node;
    list->size++;
}

static void ooc_linkedListLinkBefore(OOC_LinkedList* list,
                                     OOC_LinkedListNode* succ,
                                     OOC_LinkedListNode* node) {
    node->next = succ;
    node->prev = succ ? succ->prev : NULL;

    if (node->prev) {
        node->prev->next = node;
    } else {
        list->head = node;
    }

    if (succ) {
        succ->prev = node;
    } else {
        list->tail = node;
    }

    list->size++;
}

static void* ooc_linkedListUnlink(OOC_LinkedList* list,
                                  OOC_LinkedListNode* node,
                                  bool destroyElement) {
    if (!list || !node) {
        return NULL;
    }

    void* data = node->data;

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

    free(node);
    if (list->size > 0) {
        list->size--;
    }

    if (destroyElement) {
        ooc_destroy(data);
        return NULL;
    }

    return data;
}

static OOC_LinkedListNode* ooc_linkedListNodeAt(OOC_LinkedList* list, size_t index) {
    if (!list || index >= list->size) {
        return NULL;
    }

    if (index < (list->size >> 1)) {
        OOC_LinkedListNode* x = list->head;
        for (size_t i = 0; i < index; ++i) {
            x = x->next;
        }
        return x;
    }

    OOC_LinkedListNode* x = list->tail;
    for (size_t i = list->size - 1; i > index; --i) {
        x = x->prev;
    }
    return x;
}

static bool ooc_linkedListIteratorHasNext(void* self) {
    if (!self) {
        return false;
    }

    OOC_LinkedListIterator* it = (OOC_LinkedListIterator*)self;
    return it->nextIndex < it->list->size;
}

static void* ooc_linkedListIteratorNext(void* self) {
    if (!self) {
        return NULL;
    }

    OOC_LinkedListIterator* it = (OOC_LinkedListIterator*)self;
    if (it->nextIndex >= it->list->size || !it->next) {
        return NULL;
    }

    ooc_superBaseIteratorNext(it);

    it->lastReturned = it->next;
    it->next = it->next->next;
    it->nextIndex++;

    return it->lastReturned->data;
}

static OOC_Error ooc_linkedListIteratorRemove(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }

    OOC_LinkedListIterator* it = (OOC_LinkedListIterator*)self;

    OOC_Error e = ooc_superBaseIteratorRemove(it);
    if (e != OOC_ERROR_NONE) {
        return e;
    }

    if (!it->lastReturned) {
        return OOC_ERROR_NOT_FOUND;
    }

    OOC_LinkedListNode* node = it->lastReturned;

    /* If last op was previous(), next points to lastReturned.
       After removal cursor should point to successor. */
    if (node == it->next) {
        it->next = it->next->next;
    } else {
        /* last op was next(); cursor had advanced past removed node */
        if (it->nextIndex > 0) {
            it->nextIndex--;
        }
    }

    (void)ooc_linkedListUnlink(it->list, node, true);
    it->lastReturned = NULL;
    return OOC_ERROR_NONE;
}

static bool ooc_linkedListIteratorHasPrevious(void* self) {
    if (!self) {
        return false;
    }

    OOC_LinkedListIterator* it = (OOC_LinkedListIterator*)self;
    return it->nextIndex > 0;
}

static void* ooc_linkedListIteratorPrevious(void* self) {
    if (!self) {
        return NULL;
    }

    OOC_LinkedListIterator* it = (OOC_LinkedListIterator*)self;
    if (it->nextIndex == 0) {
        return NULL;
    }

    ooc_superBaseIteratorNext(it);

    if (it->next == NULL) {
        it->next = it->list->tail;
    } else {
        it->next = it->next->prev;
    }

    it->lastReturned = it->next;
    it->nextIndex--;

    return it->lastReturned ? it->lastReturned->data : NULL;
}

static int ooc_linkedListIteratorNextIndex(void* self) {
    if (!self) {
        return -1;
    }

    OOC_LinkedListIterator* it = (OOC_LinkedListIterator*)self;
    return (int)it->nextIndex;
}

static int ooc_linkedListIteratorPreviousIndex(void* self) {
    if (!self) {
        return -1;
    }

    OOC_LinkedListIterator* it = (OOC_LinkedListIterator*)self;
    return (it->nextIndex == 0) ? -1 : (int)(it->nextIndex - 1);
}

static OOC_Error ooc_linkedListIteratorSet(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }

    OOC_LinkedListIterator* it = (OOC_LinkedListIterator*)self;
    if (!it->lastReturned) {
        return OOC_ERROR_NOT_FOUND;
    }

    ooc_destroy(it->lastReturned->data);
    it->lastReturned->data = element;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedListIteratorAdd(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }

    OOC_LinkedListIterator* it = (OOC_LinkedListIterator*)self;
    OOC_LinkedListNode* node = ooc_linkedListCreateNode(element);
    if (!node) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }

    if (it->next == NULL) {
        ooc_linkedListLinkLast(it->list, node);
    } else {
        ooc_linkedListLinkBefore(it->list, it->next, node);
    }

    it->nextIndex++;
    it->lastReturned = NULL;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedListIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }

    OOC_LinkedListIterator* it = (OOC_LinkedListIterator*)self;
    OOC_Error e = ooc_superCtor(it, args);
    if (e != OOC_ERROR_NONE) {
        return e;
    }

    it->list = va_arg(*args, OOC_LinkedList*);
    size_t index = va_arg(*args, size_t);

    if (!it->list) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(it->list, ooc_linkedListClass(), OOC_ERROR_INVALID_OBJECT);

    if (index > it->list->size) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }

    it->nextIndex = index;
    it->lastReturned = NULL;
    it->next = (index == it->list->size) ? NULL : ooc_linkedListNodeAt(it->list, index);

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
                     OOC_BASE_ITERATOR_METHOD_HAS_NEXT, ooc_linkedListIteratorHasNext,
                     OOC_BASE_ITERATOR_METHOD_NEXT, ooc_linkedListIteratorNext,
                     OOC_BASE_ITERATOR_METHOD_REMOVE, ooc_linkedListIteratorRemove,
                     offsetof(OOC_LinkedListIteratorClass, listIteratorVtable.hasPrevious), ooc_linkedListIteratorHasPrevious,
                     offsetof(OOC_LinkedListIteratorClass, listIteratorVtable.previous), ooc_linkedListIteratorPrevious,
                     offsetof(OOC_LinkedListIteratorClass, listIteratorVtable.nextIndex), ooc_linkedListIteratorNextIndex,
                     offsetof(OOC_LinkedListIteratorClass, listIteratorVtable.previousIndex), ooc_linkedListIteratorPreviousIndex,
                     offsetof(OOC_LinkedListIteratorClass, listIteratorVtable.set), ooc_linkedListIteratorSet,
                     offsetof(OOC_LinkedListIteratorClass, listIteratorVtable.add), ooc_linkedListIteratorAdd,
                     0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&LinkedListIteratorClassInstance);
        return NULL;
    }

    LinkedListIteratorInterfaces[0].interfaceClass = ooc_listIteratorClass();
    LinkedListIteratorInterfaces[0].vtableOffset =
        offsetof(OOC_LinkedListIteratorClass, listIteratorVtable);

    if (ooc_classSetInterface(&LinkedListIteratorClassInstance,
                              LinkedListIteratorInterfaces,
                              1) != OOC_ERROR_NONE) {
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
    return ooc_collectionGetIterator(self);
}

size_t ooc_linkedListSize(void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), 0);
    OOC_LinkedList* list = (OOC_LinkedList*)self;
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

    OOC_LinkedList* list = (OOC_LinkedList*)self;
    OOC_LinkedListNode* node = ooc_linkedListCreateNode(element);
    if (!node) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }

    ooc_linkedListLinkFirst(list, node);
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

    OOC_LinkedList* list = (OOC_LinkedList*)self;
    return list->head ? list->head->data : NULL;
}

void* ooc_linkedListGetLast(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);

    OOC_LinkedList* list = (OOC_LinkedList*)self;
    return list->tail ? list->tail->data : NULL;
}

void* ooc_linkedListRemoveFirst(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);

    OOC_LinkedList* list = (OOC_LinkedList*)self;
    if (!list->head) {
        return NULL;
    }

    return ooc_linkedListUnlink(list, list->head, false);
}

void* ooc_linkedListRemoveLast(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedListClass(), NULL);

    OOC_LinkedList* list = (OOC_LinkedList*)self;
    if (!list->tail) {
        return NULL;
    }

    return ooc_linkedListUnlink(list, list->tail, false);
}

OOC_Error ooc_linkedListPush(void* self, void* element) {
    return ooc_linkedListAddLast(self, element);
}

void* ooc_linkedListPop(void* self) {
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

    OOC_LinkedList* list = (OOC_LinkedList*)self;
    OOC_Error e = ooc_superCtor(list, args);
    if (e != OOC_ERROR_NONE) {
        return e;
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

    OOC_Error e = ooc_linkedListClear(self);
    if (e != OOC_ERROR_NONE) {
        return e;
    }

    return ooc_superDtor(self);
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
                     OOC_ABSTRACT_COLLECTION_METHOD_SIZE, ooc_linkedListSize,
                     OOC_ABSTRACT_LIST_METHOD_GET_LIST_ITERATOR_AT, ooc_linkedListGetListIteratorAt,
                     OOC_LINKED_LIST_METHOD_PUSH, ooc_linkedListPush,
                     OOC_LINKED_LIST_METHOD_POP, ooc_linkedListPop,
                     OOC_LINKED_LIST_METHOD_PEEK, ooc_linkedListPeek,
                     OOC_LINKED_LIST_METHOD_ADD_FIRST, ooc_linkedListAddFirst,
                     OOC_LINKED_LIST_METHOD_ADD_LAST, ooc_linkedListAddLast,
                     OOC_LINKED_LIST_METHOD_REMOVE_FIRST, ooc_linkedListRemoveFirst,
                     OOC_LINKED_LIST_METHOD_REMOVE_LAST, ooc_linkedListRemoveLast,
                     OOC_LINKED_LIST_METHOD_GET_FIRST, ooc_linkedListGetFirst,
                     OOC_LINKED_LIST_METHOD_GET_LAST, ooc_linkedListGetLast,
                     0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&LinkedListClassInstance);
        return NULL;
    }

    LinkedListInterfaces[0].interfaceClass = ooc_dequeClass();
    LinkedListInterfaces[0].vtableOffset = offsetof(OOC_LinkedListClass, dequeVtable);

    if (ooc_classSetInterface(&LinkedListClassInstance,
                              LinkedListInterfaces,
                              1) != OOC_ERROR_NONE) {
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
