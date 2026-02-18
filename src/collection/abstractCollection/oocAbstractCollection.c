#include "oocAbstractCollection.h"
#include "oocAbstractCollection.r"
#include "oocError.h"
#include "oocObject.h"
#include "oocObject.r"
#include "oocIterable.h"
#include "oocCollection.h"
#include "oocIterator.h"
#include "oocStringBuffer.h"
#include "oocString.h"
#include <stdlib.h>
#include <string.h>

static OOC_AbstractCollectionClass* AbstractCollectionClass;
static OOC_AbstractCollectionClass AbstractCollectionClassInstance;

static OOC_InterfaceImpl AbstractCollectiionInterfaces[2];

static char* ooc_abstractCollectionToString(const void* self) {
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), NULL);
    void* it = ooc_iterableGetIterator((void*)self);
    if (!it) {
        return ooc_StringFormat("%s", "[]");
    }
    void* buf = ooc_new(ooc_stringBufferClass(), "[");
    if (!buf) {
        ooc_destroy(it);
        return NULL;
    }
    bool first = true;
    while (ooc_iteratorHasNext(it)) {
        if (!first) {
            ooc_stringBufferAppendCString(buf, ", ");
        }
        first = false;
        void* elem = ooc_iteratorNext(it);
        char* elemStr = ooc_toString(elem);
        ooc_stringBufferAppendCString(buf, elemStr ? elemStr : "null");
        free(elemStr);
    }
    ooc_stringBufferAppendCString(buf, "]");
    char* result = ooc_stringBufferToCString(buf);
    ooc_destroy(buf);
    ooc_destroy(it);
    return result;
}

static size_t ooc_abstractCollectionHash(const void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), 0);
    size_t hash = 0;
    void* it = ooc_iterableGetIterator((void*)self);
    if (!it) {
        return 0;
    }
    while (ooc_iteratorHasNext(it)) {
        void* elem = ooc_iteratorNext(it);
        hash += ooc_hashCode(elem);
    }
    ooc_destroy(it);
    return hash;
}

static void* ooc_abstractCollectionClone(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), NULL);
    void* clone = ooc_new((void*)ooc_classOf(self), (size_t)0);
    if (!clone) {
        return NULL;
    }
    void* iterator = ooc_iterableGetIterator((void*)self);
    if (!iterator) {
        return clone;
    }
    while (ooc_iteratorHasNext(iterator)) {
        void* elem = ooc_iteratorNext(iterator);
        void* elemClone = ooc_clone(elem);
        if (ooc_collectionAdd(clone, elemClone) != OOC_ERROR_NONE) {
            ooc_destroy(elemClone);
            ooc_destroy(iterator);
            ooc_destroy(clone);
            return NULL;
        }
    }
    ooc_destroy(iterator);
    return clone;
}

void* ooc_abstractCollectionGetIterator(void* self) {
    return ooc_collectionGetIterator(self);
}

size_t ooc_abstractCollectionSize(void* self) {
    return ooc_collectionSize(self);
}

bool ooc_abstractCollectionIsEmpty(void* self) {
    return ooc_collectionSize(self) == 0;
}

bool ooc_abstractCollectionContains(void* self, void* object) {
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), false);
    bool found = false;
    void* iterator = ooc_iterableGetIterator(self);
    while (ooc_iteratorHasNext(iterator)) {
        void* elem = ooc_iteratorNext(iterator);
        if (ooc_equals(elem, object)) {
            found = true;
            break;
        }
    }
    ooc_destroy(iterator);
    return found;
}

bool ooc_abstractCollectionContainsAll(void* self, void* other) {
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), false);
    OOC_TYPE_CHECK(other, ooc_abstractCollectionClass(), false);
    // If other is empty, always true (empty set is subset of any set)
    if (ooc_collectionIsEmpty(other)) {
        return true;
    }
    bool allFound = true;
    void* iterator = ooc_iterableGetIterator(other);
    while (ooc_iteratorHasNext(iterator)) {
        void* elem = ooc_iteratorNext(iterator);
        if (!ooc_collectionContains(self, elem)) {
            allFound = false;
            break;
        }
    }
    ooc_destroy(iterator);
    return allFound;
}

OOC_Error ooc_abstractCollectionAdd(void* self, void* element) {
    return OOC_ERROR_NOT_SUPPORTED;
}

OOC_Error ooc_abstractCollectionRemove(void* self, void* object) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = OOC_ERROR_NOT_FOUND;
    void* iterator = ooc_iterableGetIterator(self);
    while (ooc_iteratorHasNext(iterator)) {
        void* elem = ooc_iteratorNext(iterator);
        if (ooc_equals(elem, object)) {
            error = ooc_iteratorRemove(iterator);
            break;
        }
    }
    ooc_destroy(iterator);
    return error;
}

OOC_Error ooc_abstractCollectionClear(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = OOC_ERROR_NONE;
    void* iterator = ooc_iterableGetIterator(self);
    while (ooc_iteratorHasNext(iterator)) {
        ooc_iteratorNext(iterator);
        error = ooc_iteratorRemove(iterator);
        if (error != OOC_ERROR_NONE) {
            break;
        }
    }
    ooc_destroy(iterator);
    return error;
}

static void* ooc_abstractCollectionClassInit(void) {
    if (ooc_classNew(&AbstractCollectionClassInstance,
                    "AbstractCollection",
                    sizeof(OOC_AbstractCollection),
                    sizeof(OOC_AbstractCollectionClass),
                    ooc_objectClass(),
                    OOC_MODIFIER_ABSTRACT,
                    OOC_METHOD_TO_STRING, ooc_abstractCollectionToString,
                    OOC_METHOD_HASH, ooc_abstractCollectionHash,
                    OOC_METHOD_CLONE, ooc_abstractCollectionClone,
                    OOC_ABSTRACT_COLLECTION_METHOD_IS_EMPTY, ooc_abstractCollectionIsEmpty,
                    OOC_ABSTRACT_COLLECTION_METHOD_CONTAINS, ooc_abstractCollectionContains,
                    OOC_ABSTRACT_COLLECTION_METHOD_CONTAINS_ALL, ooc_abstractCollectionContainsAll,
                    OOC_ABSTRACT_COLLECTION_METHOD_ADD, ooc_abstractCollectionAdd,
                    OOC_ABSTRACT_COLLECTION_METHOD_REMOVE, ooc_abstractCollectionRemove,
                    OOC_ABSTRACT_COLLECTION_METHOD_CLEAR, ooc_abstractCollectionClear,
                    0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&AbstractCollectionClassInstance);
        return NULL;
    }
    AbstractCollectiionInterfaces[0].interfaceClass = ooc_iterableClass();
    AbstractCollectiionInterfaces[0].vtableOffset = offsetof(OOC_AbstractCollectionClass, iterableVtable);
    AbstractCollectiionInterfaces[1].interfaceClass = ooc_collectionClass();
    AbstractCollectiionInterfaces[1].vtableOffset = offsetof(OOC_AbstractCollectionClass, collectionVtable);
    if (ooc_classSetInterface(&AbstractCollectionClassInstance,
                        AbstractCollectiionInterfaces,
                        2) != OOC_ERROR_NONE) {
        ooc_classDestroy(&AbstractCollectionClassInstance);
        return NULL;
    }
    return &AbstractCollectionClassInstance;
}

void* ooc_abstractCollectionClass() {
    if (!AbstractCollectionClass) {
        AbstractCollectionClass = ooc_abstractCollectionClassInit();
    }
    return AbstractCollectionClass;
}

