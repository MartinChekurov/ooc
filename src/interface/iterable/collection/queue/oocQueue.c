#include "oocQueue.h"
#include "oocQueue.r"
#include "oocCollection.h"
#include "oocObject.h"
#include "oocIterable.h"
#include <stddef.h>

static OOC_QueueClass* QueueClass;
static OOC_QueueClass QueueClassInstance;

static void* ooc_queueClassInit(void) {
    if (ooc_classNew(&QueueClassInstance,
                    "Queue",
                    (size_t)0,
                    sizeof(OOC_QueueClass),
                    ooc_collectionClass(),
                    OOC_MODIFIER_ABSTRACT,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &QueueClassInstance;
}

void* ooc_queueClass() {
    if (!QueueClass) {
        QueueClass = ooc_queueClassInit();
    }
    return QueueClass;
}

void* ooc_queueGetIterator(void* self) {
    return ooc_iterableGetIterator(self);
}

size_t ooc_queueSize(void* self) {
    return ooc_collectionSize(self);
}

bool ooc_queueIsEmpty(void* self) {
    return ooc_collectionIsEmpty(self);
}

bool ooc_queueContains(void* self, void* element) {
    return ooc_collectionContains(self, element);
}

OOC_Error ooc_queueAdd(void* self, void* element) {
    return ooc_collectionAdd(self, element);
}

OOC_Error ooc_queueRemove(void* self, void* element) {
    return ooc_collectionRemove(self, element);
}

OOC_Error ooc_queueClear(void* self) {
    return ooc_collectionClear(self);
}

OOC_Error ooc_queuePush(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    const OOC_QueueVtable* vtable = ooc_getInterfaceVtable(self, ooc_queueClass());
    if (!vtable) {
        return OOC_ERROR_NO_CLASS;
    }
    if (!vtable->push) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->push(self, element);
}

void* ooc_queuePop(void* self) {
    if (!self) {
        return NULL;
    }
    const OOC_QueueVtable* vtable = ooc_getInterfaceVtable(self, ooc_queueClass());
    if (!vtable) {
        return NULL;
    }
    if (!vtable->pop) {
        return NULL;
    }
    return vtable->pop(self);
}

void* ooc_queuePeek(void* self) {
    if (!self) {
        return NULL;
    }
    const OOC_QueueVtable* vtable = ooc_getInterfaceVtable(self, ooc_queueClass());
    if (!vtable) {
        return NULL;
    }
    if (!vtable->peek) {
        return NULL;
    }
    return vtable->peek(self);
}
