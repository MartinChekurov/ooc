#include "oocBaseIterator.h"
#include "oocBaseIterator.r"
#include "oocObject.h"
#include "oocObject.r"
#include "oocIterator.h"
#include <stdbool.h>

static OOC_BaseIteratorClass* BaseIteratorClass;
static OOC_BaseIteratorClass BaseIteratorClassInstance;

static OOC_InterfaceImpl BaseIteratorInterfaces[1];

bool ooc_baseIteratorHasNext(void* self) {
    return ooc_iteratorHasNext(self);
}

void* ooc_baseIteratorNext(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_baseIteratorClass(), NULL);
    OOC_BaseIterator* iterator = self;
    iterator->canRemove = true;
    return NULL;
}

OOC_Error ooc_baseIteratorRemove(void* self) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_baseIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_BaseIterator* iterator = self;
    if (!iterator->canRemove) {
        return OOC_ERROR_INVALID_STATE;
    }
    iterator->canRemove = false;
    return OOC_ERROR_NONE;
}

void* ooc_superBaseIteratorNext(void* self) {
    return ooc_baseIteratorNext(self);
}

OOC_Error ooc_superBaseIteratorRemove(void* self) {
    return ooc_baseIteratorRemove(self);
}

bool ooc_superBaseIteratorHasNext(void* self) {
    return ooc_baseIteratorHasNext(self);
}

static OOC_Error ooc_baseIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_baseIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_BaseIterator* iterator = self;
    OOC_Error error = ooc_superCtor(iterator, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    iterator->canRemove = false;
    return OOC_ERROR_NONE;
}

static void* ooc_baseIteratorClassInit(void) {
    if (ooc_classNew(&BaseIteratorClassInstance,
                    "BaseIterator",
                    sizeof(OOC_BaseIterator),
                    sizeof(OOC_BaseIteratorClass),
                    ooc_objectClass(),
                    OOC_MODIFIER_ABSTRACT,
                    OOC_METHOD_CTOR, ooc_baseIteratorCtor,
                    OOC_BASE_ITERATOR_METHOD_NEXT, ooc_baseIteratorNext,
                    OOC_BASE_ITERATOR_METHOD_REMOVE, ooc_baseIteratorRemove,
                    0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&BaseIteratorClassInstance);
        return NULL;
    }
    BaseIteratorInterfaces[0].interfaceClass = ooc_iteratorClass();
    BaseIteratorInterfaces[0].vtableOffset = offsetof(OOC_BaseIteratorClass, iterator);
    if (ooc_classSetInterface(&BaseIteratorClassInstance,
                        BaseIteratorInterfaces,
                        1) != OOC_ERROR_NONE) {
        ooc_classDestroy(&BaseIteratorClassInstance);
        return NULL;
    }
    return &BaseIteratorClassInstance;
}

void* ooc_baseIteratorClass(void) {
    if (!BaseIteratorClass) {
        BaseIteratorClass = ooc_baseIteratorClassInit();
    }
    return BaseIteratorClass;
}
