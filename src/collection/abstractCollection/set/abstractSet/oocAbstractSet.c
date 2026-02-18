#include "oocAbstractSet.h"
#include "oocAbstractSet.r"
#include "oocAbstractCollection.h"
#include "oocCollection.h"
#include "oocIterable.h"
#include "oocIterator.h"
#include "oocStringBuffer.h"
#include "oocString.h"
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
        return ooc_StringFormat("%s", "{}");
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
    if (self == other) {
        return true;
    }
    OOC_TYPE_CHECK(self, ooc_abstractSetClass(), false);
    OOC_TYPE_CHECK(other, ooc_abstractSetClass(), false);
    size_t selfSize = ooc_collectionSize((void*)self);
    if (selfSize != ooc_collectionSize((void*)other)) {
        return false;
    }
    return ooc_collectionContainsAll((void*)self, (void*)other) &&
           ooc_collectionContainsAll((void*)other, (void*)self);
}

static void* ooc_abstractSetClassInit(void) {
    if (ooc_classNew(&AbstractSetClassInstance,
                    "AbstractSet",
                    sizeof(OOC_AbstractSet),
                    sizeof(OOC_AbstractSetClass),
                    ooc_abstractCollectionClass(),
                    OOC_MODIFIER_ABSTRACT,
                    OOC_METHOD_EQUALS,    ooc_abstractSetEquals,
                    OOC_METHOD_TO_STRING, ooc_abstractSetToString,
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
