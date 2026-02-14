#include "oocAbstractMap.h"
#include "oocAbstractMap.r"
#include "oocMap.h"
#include "oocObject.h"
#include "oocObject.r"
#include "oocIterable.h"
#include "oocIterator.h"
#include "oocStringBuffer.h"
#include <stdlib.h>
#include <string.h>

static OOC_AbstractMapClass* AbstractMapClass;
static OOC_AbstractMapClass AbstractMapClassInstance;

static OOC_InterfaceImpl AbstractMapInterfaces[1];

static bool ooc_abstractMapIsEmpty(void* self) {
    return ooc_mapSize(self) == 0;
}

static bool ooc_abstractMapContainsValue(void* self, void* value) {
    if (!self) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), false);
    void* keySet = ooc_mapKeySet(self);
    if (!keySet) {
        return false;
    }
    bool found = false;
    void* it = ooc_iterableGetIterator(keySet);
    while (ooc_iteratorHasNext(it)) {
        void* key = ooc_iteratorNext(it);
        void* val = ooc_mapGet(self, key);
        if (ooc_equals(val, value)) {
            found = true;
            break;
        }
    }
    ooc_destroy(it);
    ooc_destroy(keySet);
    return found;
}

static OOC_Error ooc_abstractMapPut(void* self, void* key, void* value) {
    return OOC_ERROR_NOT_SUPPORTED;
}

static OOC_Error ooc_abstractMapRemove(void* self, void* key) {
    return OOC_ERROR_NOT_SUPPORTED;
}

static char* ooc_abstractMapToString(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), NULL);
    void* keySet = ooc_mapKeySet((void*)self);
    if (!keySet) {
        return strdup("{}");
    }
    void* it = ooc_iterableGetIterator(keySet);
    if (!it) {
        ooc_destroy(keySet);
        return strdup("{}");
    }
    void* buf = ooc_new(ooc_stringBufferClass(), "{");
    if (!buf) {
        ooc_destroy(it);
        ooc_destroy(keySet);
        return NULL;
    }
    bool first = true;
    while (ooc_iteratorHasNext(it)) {
        if (!first) {
            ooc_stringBufferAppendCString(buf, ", ");
        }
        first = false;
        void* key = ooc_iteratorNext(it);
        char* keyStr = ooc_toString(key);
        ooc_stringBufferAppendCString(buf, keyStr ? keyStr : "null");
        free(keyStr);
        ooc_stringBufferAppendCString(buf, "=");
        void* val = ooc_mapGet((void*)self, key);
        char* valStr = ooc_toString(val);
        ooc_stringBufferAppendCString(buf, valStr ? valStr : "null");
        free(valStr);
    }
    ooc_stringBufferAppendCString(buf, "}");
    char* result = ooc_stringBufferToCString(buf);
    ooc_destroy(buf);
    ooc_destroy(it);
    ooc_destroy(keySet);
    return result;
}

static bool ooc_abstractMapEquals(const void* self, const void* other) {
    if (!self || !other) {
        return false;
    }
    if (self == other) {
        return true;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), false);
    OOC_TYPE_CHECK(other, ooc_abstractMapClass(), false);
    size_t selfSize = ooc_mapSize((void*)self);
    if (selfSize != ooc_mapSize((void*)other)) {
        return false;
    }
    void* keySet = ooc_mapKeySet((void*)self);
    if (!keySet) {
        return selfSize == 0;
    }
    bool equal = true;
    void* it = ooc_iterableGetIterator(keySet);
    while (ooc_iteratorHasNext(it)) {
        void* key = ooc_iteratorNext(it);
        if (!ooc_mapContainsKey((void*)other, key)) {
            equal = false;
            break;
        }
        void* selfVal = ooc_mapGet((void*)self, key);
        void* otherVal = ooc_mapGet((void*)other, key);
        if (!ooc_equals(selfVal, otherVal)) {
            equal = false;
            break;
        }
    }
    ooc_destroy(it);
    ooc_destroy(keySet);
    return equal;
}

static size_t ooc_abstractMapHash(const void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), 0);
    size_t hash = 0;
    void* keySet = ooc_mapKeySet((void*)self);
    if (!keySet) {
        return 0;
    }
    void* it = ooc_iterableGetIterator(keySet);
    while (ooc_iteratorHasNext(it)) {
        void* key = ooc_iteratorNext(it);
        void* val = ooc_mapGet((void*)self, key);
        hash += ooc_hashCode(key) ^ ooc_hashCode(val);
    }
    ooc_destroy(it);
    ooc_destroy(keySet);
    return hash;
}

static void* ooc_abstractMapClassInit(void) {
    if (ooc_classNew(&AbstractMapClassInstance,
                    "AbstractMap",
                    sizeof(OOC_AbstractMap),
                    sizeof(OOC_AbstractMapClass),
                    ooc_objectClass(),
                    OOC_MODIFIER_ABSTRACT,
                    OOC_METHOD_TO_STRING, ooc_abstractMapToString,
                    OOC_METHOD_EQUALS, ooc_abstractMapEquals,
                    OOC_METHOD_HASH, ooc_abstractMapHash,
                    OOC_METHOD_COMPARE, NULL,
                    OOC_ABSTRACT_MAP_METHOD_IS_EMPTY, ooc_abstractMapIsEmpty,
                    OOC_ABSTRACT_MAP_METHOD_CONTAINS_VALUE, ooc_abstractMapContainsValue,
                    OOC_ABSTRACT_MAP_METHOD_PUT, ooc_abstractMapPut,
                    OOC_ABSTRACT_MAP_METHOD_REMOVE, ooc_abstractMapRemove,
                    0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&AbstractMapClassInstance);
        return NULL;
    }
    AbstractMapInterfaces[0].interfaceClass = ooc_mapClass();
    AbstractMapInterfaces[0].vtableOffset = offsetof(OOC_AbstractMapClass, mapVtable);
    if (ooc_classSetInterface(&AbstractMapClassInstance,
                        AbstractMapInterfaces,
                        1) != OOC_ERROR_NONE) {
        ooc_classDestroy(&AbstractMapClassInstance);
        return NULL;
    }
    return &AbstractMapClassInstance;
}

void* ooc_abstractMapClass(void) {
    if (!AbstractMapClass) {
        AbstractMapClass = ooc_abstractMapClassInit();
    }
    return AbstractMapClass;
}
