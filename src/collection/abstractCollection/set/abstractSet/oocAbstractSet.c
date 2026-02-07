#include "oocAbstractSet.h"
#include "oocAbstractSet.r"
#include "oocAbstractCollection.h"
#include "oocCollection.h"
#include "oocCollection.r"
#include "oocIterable.h"
#include "oocIterator.h"
#include "oocStringBuffer.h"
#include "oocObject.h"
#include <stdlib.h>
#include <string.h>

static OOC_AbstractSetClass* AbstractSetClass;
static OOC_AbstractSetClass AbstractSetClassInstance;

static char* ooc_abstractSetToString(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_abstractSetClass(), NULL);
    void* it = ooc_iterableGetIterator((void*)self);
    if (!it) {
        return strdup("{}");
    }
    void* buf = ooc_new(ooc_stringBufferClass(), "{");
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
    ooc_stringBufferAppendCString(buf, "}");
    char* result = ooc_stringBufferToCString(buf);
    ooc_destroy(buf);
    ooc_destroy(it);
    return result;
}

static bool ooc_abstractSetEquals(const void* self, const void* other) {
    if (!self || !other) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_abstractCollectionClass(), false);
    OOC_TYPE_CHECK(other, ooc_abstractCollectionClass(), false);
    if (!ooc_superEquals(self, other)) {
        return false;
    }
    size_t selfSize = ooc_collectionSize((void*)self);
    if (selfSize != ooc_collectionSize((void*)other)) {
        return false;
    }
    return ooc_collectionContainsAll((void*)self, (void*)other) &&
           ooc_collectionContainsAll((void*)other, (void*)self);
}

static size_t ooc_abstractSetHash(const void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_abstractSetClass(), 0);
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

static void* ooc_abstractSetClassInit(void) {
    if (ooc_classNew(&AbstractSetClassInstance,
                    "AbstractSet",
                    sizeof(OOC_AbstractSet),
                    sizeof(OOC_AbstractSetClass),
                    ooc_abstractCollectionClass(),
                    OOC_MODIFIER_ABSTRACT,
                    OOC_METHOD_EQUALS,    ooc_abstractSetEquals,
                    OOC_METHOD_HASH,      ooc_abstractSetHash,
                    OOC_METHOD_TO_STRING, ooc_abstractSetToString,
                    OOC_METHOD_COMPARE, NULL,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &AbstractSetClassInstance;
}

void* ooc_abstractSetClass() {
    if (!AbstractSetClass) {
        AbstractSetClass = ooc_abstractSetClassInit();
    }
    return AbstractSetClass;
}