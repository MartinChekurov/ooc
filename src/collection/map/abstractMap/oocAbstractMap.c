#include "oocAbstractMap.h"
#include "oocAbstractMap.r"
#include "oocMap.h"
#include "oocObject.h"
#include "oocObject.r"
#include "oocHashMapEntry.h"
#include "oocIterator.h"
#include "oocHashSet.h"
#include "oocArrayList.h"
#include "oocSet.h"
#include "oocList.h"
#include "oocStringBuffer.h"
#include <stdlib.h>
#include <string.h>

static OOC_AbstractMapClass* AbstractMapClass;
static OOC_AbstractMapClass AbstractMapClassInstance;

static OOC_InterfaceImpl AbstractMapInterfaces[1];

size_t ooc_abstractMapSize(void* self) {
    return ooc_mapSize(self);
}

bool ooc_abstractMapIsEmpty(void* self) {
    return ooc_mapSize(self) == 0;
}

bool ooc_abstractMapContainsKey(void* self, void* key) {
    if (!self) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), false);
    void* it = ooc_mapGetIterator(self);
    if (!it) {
        return false;
    }
    bool found = false;
    while (ooc_iteratorHasNext(it)) {
        void* entry = ooc_iteratorNext(it);
        if (ooc_equals(ooc_hashMapEntryGetKey(entry), key)) {
            found = true;
            break;
        }
    }
    ooc_destroy(it);
    return found;
}

bool ooc_abstractMapContainsValue(void* self, void* value) {
    if (!self) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), false);
    void* it = ooc_mapGetIterator(self);
    if (!it) {
        return false;
    }
    bool found = false;
    while (ooc_iteratorHasNext(it)) {
        void* entry = ooc_iteratorNext(it);
        if (ooc_equals(ooc_hashMapEntryGetValue(entry), value)) {
            found = true;
            break;
        }
    }
    ooc_destroy(it);
    return found;
}

void* ooc_abstractMapGet(void* self, void* key) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), NULL);
    void* it = ooc_mapGetIterator(self);
    if (!it) {
        return NULL;
    }
    void* result = NULL;
    while (ooc_iteratorHasNext(it)) {
        void* entry = ooc_iteratorNext(it);
        if (ooc_equals(ooc_hashMapEntryGetKey(entry), key)) {
            result = ooc_hashMapEntryGetValue(entry);
            break;
        }
    }
    ooc_destroy(it);
    return result;
}

OOC_Error ooc_abstractMapPut(void* self, void* key, void* value) {
    return OOC_ERROR_NOT_SUPPORTED;
}

OOC_Error ooc_abstractMapRemove(void* self, void* key) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), OOC_ERROR_INVALID_OBJECT);
    void* it = ooc_mapGetIterator(self);
    if (!it) {
        return OOC_ERROR_NOT_FOUND;
    }
    OOC_Error error = OOC_ERROR_NOT_FOUND;
    while (ooc_iteratorHasNext(it)) {
        void* entry = ooc_iteratorNext(it);
        if (ooc_equals(ooc_hashMapEntryGetKey(entry), key)) {
            error = ooc_iteratorRemove(it);
            break;
        }
    }
    ooc_destroy(it);
    return error;
}

OOC_Error ooc_abstractMapClear(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), OOC_ERROR_INVALID_OBJECT);
    void* it = ooc_mapGetIterator(self);
    if (!it) {
        return OOC_ERROR_NONE;
    }
    OOC_Error error = OOC_ERROR_NONE;
    while (ooc_iteratorHasNext(it)) {
        ooc_iteratorNext(it);
        error = ooc_iteratorRemove(it);
        if (error != OOC_ERROR_NONE) {
            break;
        }
    }
    ooc_destroy(it);
    return error;
}

void* ooc_abstractMapKeySet(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), NULL);
    void* keySet = ooc_new(ooc_hashSetClass(), ooc_mapSize(self));
    if (!keySet) {
        return NULL;
    }
    void* it = ooc_mapGetIterator(self);
    if (!it) {
        ooc_destroy(keySet);
        return NULL;
    }
    while (ooc_iteratorHasNext(it)) {
        void* entry = ooc_iteratorNext(it);
        ooc_setAdd(keySet, ooc_hashMapEntryGetKey(entry));
    }
    ooc_destroy(it);
    return keySet;
}

void* ooc_abstractMapValues(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), NULL);
    void* values = ooc_new(ooc_arrayListClass(), ooc_mapSize(self));
    if (!values) {
        return NULL;
    }
    void* it = ooc_mapGetIterator(self);
    if (!it) {
        ooc_destroy(values);
        return NULL;
    }
    while (ooc_iteratorHasNext(it)) {
        void* entry = ooc_iteratorNext(it);
        ooc_listAdd(values, ooc_hashMapEntryGetValue(entry));
    }
    ooc_destroy(it);
    return values;
}

void* ooc_abstractMapGetIterator(void* self) {
    return ooc_mapGetIterator(self);
}

static char* ooc_abstractMapToString(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), NULL);
    void* it = ooc_mapGetIterator((void*)self);
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
        void* entry = ooc_iteratorNext(it);
        char* keyStr = ooc_toString(ooc_hashMapEntryGetKey(entry));
        ooc_stringBufferAppendCString(buf, keyStr ? keyStr : "null");
        free(keyStr);
        ooc_stringBufferAppendCString(buf, "=");
        char* valStr = ooc_toString(ooc_hashMapEntryGetValue(entry));
        ooc_stringBufferAppendCString(buf, valStr ? valStr : "null");
        free(valStr);
    }
    ooc_stringBufferAppendCString(buf, "}");
    char* result = ooc_stringBufferToCString(buf);
    ooc_destroy(buf);
    ooc_destroy(it);
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
    void* it = ooc_mapGetIterator((void*)self);
    if (!it) {
        return selfSize == 0;
    }
    bool equal = true;
    while (ooc_iteratorHasNext(it)) {
        void* entry = ooc_iteratorNext(it);
        void* key = ooc_hashMapEntryGetKey(entry);
        if (!ooc_mapContainsKey((void*)other, key)) {
            equal = false;
            break;
        }
        void* selfVal = ooc_hashMapEntryGetValue(entry);
        void* otherVal = ooc_mapGet((void*)other, key);
        if (!ooc_equals(selfVal, otherVal)) {
            equal = false;
            break;
        }
    }
    ooc_destroy(it);
    return equal;
}

static size_t ooc_abstractMapHash(const void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), 0);
    void* it = ooc_mapGetIterator((void*)self);
    if (!it) {
        return 0;
    }
    size_t hash = 0;
    while (ooc_iteratorHasNext(it)) {
        void* entry = ooc_iteratorNext(it);
        hash += ooc_hashCode(ooc_hashMapEntryGetKey(entry)) ^ ooc_hashCode(ooc_hashMapEntryGetValue(entry));
    }
    ooc_destroy(it);
    return hash;
}

static void* ooc_abstractMapClone(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_abstractMapClass(), NULL);
    void* clone = ooc_new((void*)ooc_classOf(self), (size_t)0);
    if (!clone) {
        return NULL;
    }
    void* it = ooc_mapGetIterator((void*)self);
    if (!it) {
        return clone;
    }
    while (ooc_iteratorHasNext(it)) {
        void* entry = ooc_iteratorNext(it);
        void* key = ooc_clone(ooc_hashMapEntryGetKey(entry));
        void* value = ooc_clone(ooc_hashMapEntryGetValue(entry));
        if (ooc_mapPut(clone, key, value) != OOC_ERROR_NONE) {
            ooc_destroy(it);
            ooc_destroy(clone);
            return NULL;
        }
    }
    ooc_destroy(it);
    return clone;
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
                    OOC_METHOD_CLONE, ooc_abstractMapClone,
                    OOC_ABSTRACT_MAP_METHOD_IS_EMPTY, ooc_abstractMapIsEmpty,
                    OOC_ABSTRACT_MAP_METHOD_CONTAINS_KEY, ooc_abstractMapContainsKey,
                    OOC_ABSTRACT_MAP_METHOD_CONTAINS_VALUE, ooc_abstractMapContainsValue,
                    OOC_ABSTRACT_MAP_METHOD_GET, ooc_abstractMapGet,
                    OOC_ABSTRACT_MAP_METHOD_PUT, ooc_abstractMapPut,
                    OOC_ABSTRACT_MAP_METHOD_REMOVE, ooc_abstractMapRemove,
                    OOC_ABSTRACT_MAP_METHOD_CLEAR, ooc_abstractMapClear,
                    OOC_ABSTRACT_MAP_METHOD_KEY_SET, ooc_abstractMapKeySet,
                    OOC_ABSTRACT_MAP_METHOD_VALUES, ooc_abstractMapValues,
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
