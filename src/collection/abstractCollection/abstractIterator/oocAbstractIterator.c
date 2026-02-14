#include "oocAbstractIterator.h"
#include "oocAbstractIterator.r"
#include "oocObject.h"
#include "oocObject.r"
#include "oocIterator.h"
#include <stdbool.h>

static OOC_AbstractIteratorClass* AbstractIteratorClass;
static OOC_AbstractIteratorClass AbstractIteratorClassInstance;

static OOC_InterfaceImpl AbstractIteratorInterfaces[1];

bool ooc_abstractIteratorHasNext(void* self) {
    return ooc_iteratorHasNext(self);
}

void* ooc_abstractIteratorNext(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_abstractIteratorClass(), NULL);
    OOC_AbstractIterator* iterator = self;
    iterator->canModify = true;
    return NULL;
}

OOC_Error ooc_abstractIteratorRemove(void* self) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_abstractIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_AbstractIterator* iterator = self;
    if (!iterator->canModify) {
        return OOC_ERROR_INVALID_STATE;
    }
    iterator->canModify = false;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_abstractIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_abstractIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_AbstractIterator* iterator = self;
    OOC_Error error = ooc_superCtor(iterator, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    iterator->canModify = false;
    return OOC_ERROR_NONE;
}

static void* ooc_abstractIteratorClassInit(void) {
    if (ooc_classNew(&AbstractIteratorClassInstance,
                    "AbstractIterator",
                    sizeof(OOC_AbstractIterator),
                    sizeof(OOC_AbstractIteratorClass),
                    ooc_objectClass(),
                    OOC_MODIFIER_ABSTRACT,
                    OOC_METHOD_CTOR, ooc_abstractIteratorCtor,
                    OOC_ABSTRACT_ITERATOR_METHOD_HAS_NEXT, NULL,
                    OOC_ABSTRACT_ITERATOR_METHOD_NEXT, ooc_abstractIteratorNext,
                    OOC_ABSTRACT_ITERATOR_METHOD_REMOVE, ooc_abstractIteratorRemove,
                    0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&AbstractIteratorClassInstance);
        return NULL;
    }
    AbstractIteratorInterfaces[0].interfaceClass = ooc_iteratorClass();
    AbstractIteratorInterfaces[0].vtableOffset = offsetof(OOC_AbstractIteratorClass, iterator);
    if (ooc_classSetInterface(&AbstractIteratorClassInstance,
                        AbstractIteratorInterfaces,
                        1) != OOC_ERROR_NONE) {
        ooc_classDestroy(&AbstractIteratorClassInstance);
        return NULL;
    }
    return &AbstractIteratorClassInstance;
}

void* ooc_abstractIteratorClass(void) {
    if (!AbstractIteratorClass) {
        AbstractIteratorClass = ooc_abstractIteratorClassInit();
    }
    return AbstractIteratorClass;
}
