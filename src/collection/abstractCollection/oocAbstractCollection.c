#include "oocAbstractCollection.h"
#include "oocAbstractCollection.r"
#include "oocError.h"
#include "oocObject.h"
#include "oocObject.r"
#include "oocIterable.h"
#include "oocCollection.h"
#include "oocIterator.h"
#include "oocStringBuffer.h"
#include <stdlib.h>
#include <string.h>

static OOC_AbstractCollectionClass* AbstractCollectionClass;
static OOC_AbstractCollectionClass AbstractCollectionClassInstance;

static OOC_InterfaceImpl AbstractCollectiionInterfaces[1];

static char* ooc_abstractCollectionToString(const void* self) {
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), NULL);
    void* it = ooc_iterableGetIterator((void*)self);
    if (!it) {
        return strdup("[]");
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

static bool ooc_abstractCollectionEquals(const void* self, const void* other) {
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), false);
    OOC_TYPE_CHECK(other, ooc_abstractCollectionClass(), false);
    size_t selfSize = ooc_collectionSize((void*)self);
    size_t otherSize = ooc_collectionSize((void*)other);
    if (selfSize != otherSize) {
        return false;
    }
    void* it1 = ooc_iterableGetIterator((void*)self);
    void* it2 = ooc_iterableGetIterator((void*)other);
    if (!it1 || !it2) {
        ooc_destroy(it1);
        ooc_destroy(it2);
        return false;
    }
    while (ooc_iteratorHasNext(it1) && ooc_iteratorHasNext(it2)) {
        void* elem1 = ooc_iteratorNext(it1);
        void* elem2 = ooc_iteratorNext(it2);
        if (!ooc_equals(elem1, elem2)) {
            ooc_destroy(it1);
            ooc_destroy(it2);
            return false;
        }
    }
    ooc_destroy(it1);
    ooc_destroy(it2);
    return true;
}

static size_t ooc_abstractCollectionHash(const void* self) {
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), 0);
    size_t hash = 0;
    void* it = ooc_iterableGetIterator((void*)self);
    while (ooc_iteratorHasNext(it)) {
        void* elem = ooc_iteratorNext(it);
        hash = 31 * hash + ooc_hashCode(elem);
    }
    ooc_destroy(it);
    return hash;
}

static int ooc_abstractCollectionCompare(const void* self, const void* other) {
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), -1);
    OOC_TYPE_CHECK(other, ooc_abstractCollectionClass(), -1);
    void* it1 = ooc_iterableGetIterator((void*)self);
    void* it2 = ooc_iterableGetIterator((void*)other);
    if (!it1 || !it2) {
        ooc_destroy(it1);
        ooc_destroy(it2);
        return -1;
    }
    while (ooc_iteratorHasNext(it1) && ooc_iteratorHasNext(it2)) {
        void* elem1 = ooc_iteratorNext(it1);
        void* elem2 = ooc_iteratorNext(it2);
        int cmp = ooc_compare(elem1, elem2);
        if (cmp) {
            ooc_destroy(it1);
            ooc_destroy(it2);
            return cmp;
        }
    }
    int result = ooc_iteratorHasNext(it1) ? 1 : (ooc_iteratorHasNext(it2) ? -1 : 0);
    ooc_destroy(it1);
    ooc_destroy(it2);
    return result;
}

static void* ooc_abstractCollectionClone(const void* self) {
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), NULL);
    void* clone = ooc_superClone(self);
    if (!clone) {
        return NULL;
    }
    void* it = ooc_iterableGetIterator((void*)self);
    while (ooc_iteratorHasNext(it)) {
        void* elem = ooc_iteratorNext(it);
        void* elemClone = ooc_clone(elem);
        if (ooc_collectionAdd(clone, elemClone) != OOC_ERROR_NONE) {
            ooc_destroy(elemClone);
            ooc_destroy(it);
            ooc_destroy(clone);
            return NULL;
        }
    }
    ooc_destroy(it);
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
    void* it = ooc_iterableGetIterator(self);
    while (ooc_iteratorHasNext(it)) {
        void* elem = ooc_iteratorNext(it);
        if (ooc_equals(elem, object)) {
            found = true;
            break;
        }
    }
    ooc_destroy(it);
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
    void* it = ooc_iterableGetIterator(other);
    while (ooc_iteratorHasNext(it)) {
        void* elem = ooc_iteratorNext(it);
        if (!ooc_collectionContains(self, elem)) {
            allFound = false;
            break;
        }
    }
    ooc_destroy(it);
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
    bool found = false;
    void* it = ooc_iterableGetIterator(self);
    while (ooc_iteratorHasNext(it)) {
        void* elem = ooc_iteratorNext(it);
        if (ooc_equals(elem, object)) {
            ooc_iteratorRemove(it);
            found = true;
            break;
        }
    }
    ooc_destroy(it);
    return found ? OOC_ERROR_NONE : OOC_ERROR_NOT_FOUND;
}

OOC_Error ooc_abstractCollectionClear(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), OOC_ERROR_INVALID_OBJECT);
    void* iterator = ooc_iterableGetIterator(self);
    while (ooc_iteratorHasNext(iterator)) {
        ooc_iteratorNext(iterator);
        ooc_iteratorRemove(iterator);
    }
    ooc_destroy(iterator);
    return OOC_ERROR_NONE;
}

static void* ooc_abstractCollectionClassInit(void) {
    if (ooc_classNew(&AbstractCollectionClassInstance,
                    "AbstractCollection",
                    sizeof(OOC_AbstractCollection),
                    sizeof(OOC_AbstractCollectionClass),
                    ooc_objectClass(),
                    OOC_MODIFIER_ABSTRACT,
                    OOC_METHOD_TO_STRING, ooc_abstractCollectionToString,
                    OOC_METHOD_EQUALS, ooc_abstractCollectionEquals,
                    OOC_METHOD_HASH, ooc_abstractCollectionHash,
                    OOC_METHOD_COMPARE, ooc_abstractCollectionCompare,
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
    AbstractCollectiionInterfaces[0].interfaceClass = ooc_collectionClass();
    AbstractCollectiionInterfaces[0].vtableOffset = offsetof(OOC_AbstractCollectionClass, collectionVtable);
    if (ooc_classSetInterface(&AbstractCollectionClassInstance,
                        AbstractCollectiionInterfaces,
                        1) != OOC_ERROR_NONE) {
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

