#include "oocDeque.h"
#include "oocDeque.r"
#include "oocError.h"
#include "oocQueue.h"
#include "oocObject.h"
#include "oocCollection.h"
#include "oocIterable.h"
#include <stddef.h>

static OOC_DequeClass* DequeClass;
static OOC_DequeClass DequeClassInstance;

static void* ooc_dequeClassInit(void) {
    if (ooc_classNew(&DequeClassInstance,
                    "Deque",
                    (size_t)0,
                    sizeof(OOC_DequeClass),
                    ooc_queueClass(),
                    OOC_MODIFIER_ABSTRACT,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &DequeClassInstance;
}

void* ooc_queueGetIterator(void* self) {
    return ooc_iterableGetIterator(self);
}

size_t ooc_dequeSize(void* self) {
    return ooc_collectionSize(self);
}

bool ooc_dequeIsEmpty(void* self) {
    return ooc_collectionIsEmpty(self);
}

bool ooc_dequeContains(void* self, void* element) {
    return ooc_collectionContains(self, element);
}

bool ooc_dequeContainsAll(void* self, void* other) {
    return ooc_collectionContainsAll(self, other);
}

OOC_Error ooc_dequeAdd(void* self, void* element) {
    return ooc_collectionAdd(self, element);
}

OOC_Error ooc_dequeRemove(void* self, void* element) {
    return ooc_collectionRemove(self, element);
}

OOC_Error ooc_dequeClear(void* self) {
    return ooc_collectionClear(self);
}

OOC_Error ooc_dequeOffer(void* self, void* element) {
    return ooc_queueOffer(self, element);
}

void* ooc_dequePoll(void* self) {
    return ooc_queuePoll(self);
}

void* ooc_dequePeek(void* self) {
    return ooc_queuePeek(self);
}

void* ooc_dequeClass() {
    if (!DequeClass) {
        DequeClass = ooc_dequeClassInit();
    }
    return DequeClass;
}

OOC_Error ooc_dequeAddFirst(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    const OOC_DequeVtable* vtable = ooc_getInterfaceVtable(self, ooc_dequeClass());
    if (!vtable) {
        return OOC_ERROR_NO_CLASS;
    }
    if (!vtable->addFirst) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->addFirst(self, element);
}

OOC_Error ooc_dequeAddLast(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    const OOC_DequeVtable* vtable = ooc_getInterfaceVtable(self, ooc_dequeClass());
    if (!vtable) {
        return OOC_ERROR_NO_CLASS;
    }
    if (!vtable->addLast) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->addLast(self, element);
}

void* ooc_dequeRemoveFirst(void* self) {
    if (!self) {
        return NULL;
    }
    const OOC_DequeVtable* vtable = ooc_getInterfaceVtable(self, ooc_dequeClass());
    if (!vtable) {
        return NULL;
    }
    if (!vtable->removeFirst) {
        return NULL;
    }
    return vtable->removeFirst(self);
}

void* ooc_dequeRemoveLast(void* self) {
    if (!self) {
        return NULL;
    }
    const OOC_DequeVtable* vtable = ooc_getInterfaceVtable(self, ooc_dequeClass());
    if (!vtable) {
        return NULL;
    }
    if (!vtable->removeLast) {
        return NULL;
    }
    return vtable->removeLast(self);
}

void* ooc_dequeGetFirst(void* self) {
    if (!self) {
        return NULL;
    }
    const OOC_DequeVtable* vtable = ooc_getInterfaceVtable(self, ooc_dequeClass());
    if (!vtable) {
        return NULL;
    }
    if (!vtable->getFirst) {
        return NULL;
    }
    return vtable->getFirst(self);
}

void* ooc_dequeGetLast(void* self) {
    if (!self) {
        return NULL;
    }
    const OOC_DequeVtable* vtable = ooc_getInterfaceVtable(self, ooc_dequeClass());
    if (!vtable) {
        return NULL;
    }
    if (!vtable->getLast) {
        return NULL;
    }
    return vtable->getLast(self);
}

OOC_Error ooc_dequePush(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_DequeVtable* vtable = ooc_getInterfaceVtable(self, ooc_dequeClass());
    if (!vtable) {
        return OOC_ERROR_NO_CLASS;
    }
    if (!vtable->push) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->push(self, element);
}

void* ooc_dequeGetPop(void* self) {
    if (!self) {
        return NULL;
    }
    const OOC_DequeVtable* vtable = ooc_getInterfaceVtable(self, ooc_dequeClass());
    if (!vtable) {
        return NULL;
    }
    if (!vtable->pop) {
        return NULL;
    }
    return vtable->pop(self);
}
