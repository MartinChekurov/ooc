#include "oocLinkedHashMap.h"
#include "oocLinkedHashMap.r"
#include "oocHashMap.h"
#include "oocHashMapEntry.h"
#include "oocMap.h"
#include "oocMap.r"
#include "oocList.h"
#include "oocLinkedList.h"
#include "oocLinkedHashSet.h"
#include "oocArrayList.h"
#include "oocObject.h"
#include "oocObject.r"
#include "oocIterator.h"
#include "oocIterator.r"
#include "oocSet.h"
#include "oocBaseIterator.h"
#include "oocBaseIterator.r"
#include <string.h>

typedef struct OOC_LinkedHashMapKeyIterator OOC_LinkedHashMapKeyIterator;
typedef struct OOC_LinkedHashMapKeyIteratorClass OOC_LinkedHashMapKeyIteratorClass;

struct OOC_LinkedHashMapKeyIterator {
    OOC_BaseIterator object;
    void* listIterator;
};

struct OOC_LinkedHashMapKeyIteratorClass {
    OOC_BaseIteratorClass class;
};

static OOC_LinkedHashMapClass* LinkedHashMapClass;
static OOC_LinkedHashMapClass LinkedHashMapClassInstance;

static OOC_LinkedHashMapKeyIteratorClass* LinkedHashMapKeyIteratorClass;
static OOC_LinkedHashMapKeyIteratorClass LinkedHashMapKeyIteratorClassInstance;

static OOC_Error ooc_linkedHashMapPut(void* self, void* key, void* value) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMap* map = self;
    OOC_Error error = ooc_mapPut(map, key, value);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    void* entry = ooc_new(ooc_hashMapEntryClass(), NULL, NULL);
    if (!entry) {
        ooc_mapRemove(map, key);
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    error = ooc_listAdd(map->insertionOrder, entry);
    if (error != OOC_ERROR_NONE) {
        ooc_destroy(entry);
        ooc_mapRemove(map, key);
        return error;
    }
    ooc_hashMapEntrySetKey(entry, key);
    ooc_hashMapEntrySetValue(entry, value);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedHashMapRemove(void* self, void* key) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMap* map = self;
    OOC_Error error = ooc_mapRemove(map, key);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    void* listIterator = ooc_listGetIterator(map->insertionOrder);
    while (ooc_iteratorHasNext(listIterator)) {
        void* entry = ooc_iteratorNext(listIterator);
        if (ooc_equals(ooc_hashMapEntryGetKey(entry), key)) {
            ooc_iteratorRemove(listIterator);
            ooc_destroy(listIterator);
            return OOC_ERROR_NONE;
        }
    }
    ooc_destroy(listIterator);
    return error;
}

static OOC_Error ooc_linkedHashMapClear(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMap* map = self;
    ooc_mapClear(map);
    ooc_destroy(map->insertionOrder);
    map->insertionOrder = NULL;
    return OOC_ERROR_NONE;
}

static void* ooc_linkedHashMapKeySet(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapClass(), NULL);
    OOC_LinkedHashMap* map = self;
    void* keySet = ooc_new(ooc_linkedHashSetClass(), ooc_mapSize(map));
    if (!keySet) {
        return NULL;
    }
    void* listIterator = ooc_listGetIterator(map->insertionOrder);
    while (ooc_iteratorHasNext(listIterator)) {
        void* entry = ooc_iteratorNext(listIterator);
        ooc_setAdd(keySet, ooc_hashMapEntryGetKey(entry));
    }
    ooc_destroy(listIterator);
    return keySet;
}

static void* ooc_linkedHashMapValues(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapClass(), NULL);
    OOC_LinkedHashMap* map = self;
    void* values = ooc_new(ooc_arrayListClass(), ooc_mapSize(map));
    if (!values) {
        return NULL;
    }
    void* listIterator = ooc_listGetIterator(map->insertionOrder);
    while (ooc_iteratorHasNext(listIterator)) {
        void* entry = ooc_iteratorNext(listIterator);
        ooc_listAdd(values, ooc_hashMapEntryGetValue(entry));
    }
    ooc_destroy(listIterator);
    return values;
}

static bool ooc_linkedHashMapKeyIteratorHasNext(void* self) {
    if (!self) {
        return false;
    }
    OOC_TYPE_CHECK(self, LinkedHashMapKeyIteratorClass, false);
    OOC_LinkedHashMapKeyIterator* iterator = self;
    return ooc_iteratorHasNext(iterator->listIterator);
}

static void* ooc_linkedHashMapKeyIteratorNext(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, LinkedHashMapKeyIteratorClass, NULL);
    OOC_LinkedHashMapKeyIterator* iterator = self;
    ooc_superBaseIteratorNext(iterator);
    return ooc_iteratorNext(iterator->listIterator);
}

static OOC_Error ooc_linkedHashMapKeyIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, LinkedHashMapKeyIteratorClass, OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMapKeyIterator* iterator = self;
    OOC_Error error = ooc_superCtor(iterator, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    void* iteratorInterface = ooc_new(ooc_iteratorClass(),
                                  OOC_ITERATOR_METHOD_HAS_NEXT, ooc_linkedHashMapKeyIteratorHasNext,
                                  OOC_ITERATOR_METHOD_NEXT, ooc_linkedHashMapKeyIteratorNext,
                                  0);
    error = ooc_addInterface(iterator, iteratorInterface);
    if (error != OOC_ERROR_NONE) {
        ooc_destroy(iteratorInterface);
        return error;
    }
    OOC_LinkedHashMap* map = va_arg(*args, OOC_LinkedHashMap*);
    if (!map) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    iterator->listIterator = ooc_listGetIterator(map->insertionOrder);
    if (!iterator->listIterator) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedHashMapKeyIteratorDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, LinkedHashMapKeyIteratorClass, OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMapKeyIterator* iterator = self;
    ooc_destroy(iterator->listIterator);
    iterator->listIterator = NULL;
    return ooc_superDtor(self);
}

static void* ooc_linkedHashMapKeyIteratorClassInit(void) {
    if (ooc_classNew(&LinkedHashMapKeyIteratorClassInstance,
                    "LinkedHashMapKeyIterator",
                    sizeof(OOC_LinkedHashMapKeyIterator),
                    sizeof(OOC_LinkedHashMapKeyIteratorClass),
                    ooc_baseIteratorClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_linkedHashMapKeyIteratorCtor,
                    OOC_METHOD_DTOR, ooc_linkedHashMapKeyIteratorDtor,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &LinkedHashMapKeyIteratorClassInstance;
}

void* ooc_linkedHashMapKeyIteratorClass(void) {
    if (!LinkedHashMapKeyIteratorClass) {
        LinkedHashMapKeyIteratorClass = ooc_linkedHashMapKeyIteratorClassInit();
    }
    return LinkedHashMapKeyIteratorClass;
}

void* ooc_linkedHashMapGetKeyIterator(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapClass(), NULL);
    return ooc_new(ooc_linkedHashMapKeyIteratorClass(), self);
}

static OOC_Error ooc_linkedHashMapCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMap* map = self;
    OOC_Error error = ooc_superCtor(map, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    ooc_overrideInterfaceFunctions(self, ooc_mapClass(),
                                OOC_MAP_METHOD_PUT, ooc_linkedHashMapPut,
                                OOC_MAP_METHOD_REMOVE, ooc_linkedHashMapRemove,
                                OOC_MAP_METHOD_CLEAR, ooc_linkedHashMapClear,
                                OOC_MAP_METHOD_KEY_SET, ooc_linkedHashMapKeySet,
                                OOC_MAP_METHOD_VALUES, ooc_linkedHashMapValues,
                                0);
    map->insertionOrder = ooc_new(ooc_linkedListClass());
    if (!map->insertionOrder) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedHashMapDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMap* map = self;
    ooc_destroy(map->insertionOrder);
    map->insertionOrder = NULL;
    return ooc_superDtor(map);
}

static void* ooc_linkedHashMapClassInit(void) {
    if (ooc_classNew(&LinkedHashMapClassInstance,
                    "LinkedHashMap",
                    sizeof(OOC_LinkedHashMap),
                    sizeof(OOC_LinkedHashMapClass),
                    ooc_hashMapClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_linkedHashMapCtor,
                    OOC_METHOD_DTOR, ooc_linkedHashMapDtor,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &LinkedHashMapClassInstance;
}

void* ooc_linkedHashMapClass() {
    if (!LinkedHashMapClass) {
        LinkedHashMapClass = ooc_linkedHashMapClassInit();
    }
    return LinkedHashMapClass;
}
