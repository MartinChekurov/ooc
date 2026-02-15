#include "oocLinkedHashMap.h"
#include "oocLinkedHashMap.r"
#include "oocHashMap.h"
#include "oocHashMapEntry.h"
#include "oocAbstractMap.r"
#include "oocMap.h"
#include "oocList.h"
#include "oocLinkedList.h"
#include "oocObject.h"
#include "oocObject.r"
#include "oocIterator.h"
#include "oocAbstractIterator.h"
#include "oocAbstractIterator.r"
#include <stddef.h>
#include <string.h>

typedef struct OOC_LinkedHashMapIterator OOC_LinkedHashMapIterator;
typedef struct OOC_LinkedHashMapIteratorClass OOC_LinkedHashMapIteratorClass;

struct OOC_LinkedHashMapIterator {
    OOC_AbstractIterator object;
    OOC_LinkedHashMap* map;
    void* listIterator;
    void* lastEntry;
};

struct OOC_LinkedHashMapIteratorClass {
    OOC_AbstractIteratorClass class;
};

static OOC_LinkedHashMapClass* LinkedHashMapClass;
static OOC_LinkedHashMapClass LinkedHashMapClassInstance;

static OOC_LinkedHashMapIteratorClass* LinkedHashMapIteratorClass;
static OOC_LinkedHashMapIteratorClass LinkedHashMapIteratorClassInstance;

void* ooc_linkedHashMapIteratorClass(void);

size_t ooc_linkedHashMapSize(void* self) {
    return ooc_mapSize(self);
}

bool ooc_linkedHashMapIsEmpty(void* self) {
    return ooc_mapIsEmpty(self);
}

bool ooc_linkedHashMapContainsKey(void* self, void* key) {
    return ooc_mapContainsKey(self, key);
}

bool ooc_linkedHashMapContainsValue(void* self, void* value) {
    return ooc_mapContainsValue(self, value);
}

void* ooc_linkedHashMapGet(void* self, void* key) {
    return ooc_hashMapGet(self, key);
}

OOC_Error ooc_linkedHashMapPut(void* self, void* key, void* value) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMap* map = self;
    bool exists = ooc_mapContainsKey(self, key);
    OOC_Error error = ooc_hashMapPut(self, key, value);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    if (!exists) {
        void* entry = ooc_new(ooc_hashMapEntryClass(), ooc_clone(key), ooc_clone(value));
        if (!entry) {
            ooc_hashMapRemove(self, key);
            return OOC_ERROR_OUT_OF_MEMORY;
        }
        error = ooc_listAdd(map->insertionOrder, entry);
        if (error != OOC_ERROR_NONE) {
            ooc_destroy(entry);
            ooc_hashMapRemove(self, key);
            return error;
        }
    } else {
        void* listIterator = ooc_listGetIterator(map->insertionOrder);
        while (ooc_iteratorHasNext(listIterator)) {
            void* orderEntry = ooc_iteratorNext(listIterator);
            if (ooc_equals(ooc_hashMapEntryGetKey(orderEntry), key)) {
                ooc_hashMapEntrySetValue(orderEntry, ooc_clone(value));
                break;
            }
        }
        ooc_destroy(listIterator);
    }
    return OOC_ERROR_NONE;
}

OOC_Error ooc_linkedHashMapRemove(void* self, void* key) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMap* map = self;
    void* listIterator = ooc_listGetIterator(map->insertionOrder);
    while (ooc_iteratorHasNext(listIterator)) {
        void* entry = ooc_iteratorNext(listIterator);
        if (ooc_equals(ooc_hashMapEntryGetKey(entry), key)) {
            ooc_hashMapRemove(self, key);
            ooc_iteratorRemove(listIterator);
            ooc_destroy(listIterator);
            return OOC_ERROR_NONE;
        }
    }
    ooc_destroy(listIterator);
    return OOC_ERROR_NOT_FOUND;
}

OOC_Error ooc_linkedHashMapClear(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMap* map = self;
    OOC_Error error = ooc_hashMapClear(self);
    void* listIterator = ooc_listGetIterator(map->insertionOrder);
    while (ooc_iteratorHasNext(listIterator)) {
        ooc_iteratorNext(listIterator);
        ooc_iteratorRemove(listIterator);
    }
    ooc_destroy(listIterator);
    return error;
}

void* ooc_linkedHashMapKeySet(void* self) {
    return ooc_mapKeySet(self);
}

void* ooc_linkedHashMapValues(void* self) {
    return ooc_mapValues(self);
}

static bool ooc_linkedHashMapIteratorHasNext(void* self) {
    if (!self) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapIteratorClass(), false);
    OOC_LinkedHashMapIterator* iterator = self;
    return ooc_iteratorHasNext(iterator->listIterator);
}

static void* ooc_linkedHashMapIteratorNext(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapIteratorClass(), NULL);
    OOC_LinkedHashMapIterator* iterator = self;
    ooc_abstractIteratorNext(iterator);
    iterator->lastEntry = ooc_iteratorNext(iterator->listIterator);
    return iterator->lastEntry;
}

static OOC_Error ooc_linkedHashMapIteratorRemove(void* self) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMapIterator* iterator = self;
    OOC_Error error = ooc_abstractIteratorRemove(iterator);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    void* key = ooc_hashMapEntryGetKey(iterator->lastEntry);
    error = ooc_hashMapRemove(iterator->map, key);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    error = ooc_iteratorRemove(iterator->listIterator);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    iterator->lastEntry = NULL;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedHashMapIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMapIterator* iterator = self;
    OOC_Error error = ooc_superCtor(ooc_linkedHashMapIteratorClass(), iterator, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    iterator->map = va_arg(*args, OOC_LinkedHashMap*);
    if (!iterator->map) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    iterator->lastEntry = NULL;
    iterator->listIterator = ooc_listGetIterator(iterator->map->insertionOrder);
    if (!iterator->listIterator) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_linkedHashMapIteratorDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMapIterator* iterator = self;
    ooc_destroy(iterator->listIterator);
    iterator->listIterator = NULL;
    return ooc_superDtor(ooc_linkedHashMapIteratorClass(), self);
}

static void* ooc_linkedHashMapIteratorClassInit(void) {
    if (ooc_classNew(&LinkedHashMapIteratorClassInstance,
                    "LinkedHashMapIterator",
                    sizeof(OOC_LinkedHashMapIterator),
                    sizeof(OOC_LinkedHashMapIteratorClass),
                    ooc_abstractIteratorClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_linkedHashMapIteratorCtor,
                    OOC_METHOD_DTOR, ooc_linkedHashMapIteratorDtor,
                    OOC_ABSTRACT_ITERATOR_METHOD_HAS_NEXT, ooc_linkedHashMapIteratorHasNext,
                    OOC_ABSTRACT_ITERATOR_METHOD_NEXT, ooc_linkedHashMapIteratorNext,
                    OOC_ABSTRACT_ITERATOR_METHOD_REMOVE, ooc_linkedHashMapIteratorRemove,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &LinkedHashMapIteratorClassInstance;
}

void* ooc_linkedHashMapIteratorClass(void) {
    if (!LinkedHashMapIteratorClass) {
        LinkedHashMapIteratorClass = ooc_linkedHashMapIteratorClassInit();
    }
    return LinkedHashMapIteratorClass;
}

void* ooc_linkedHashMapGetIterator(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapClass(), NULL);
    return ooc_new(ooc_linkedHashMapIteratorClass(), self);
}

static OOC_Error ooc_linkedHashMapCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashMap* map = self;
    OOC_Error error = ooc_superCtor(ooc_linkedHashMapClass(), map, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
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
    return ooc_superDtor(ooc_linkedHashMapClass(), map);
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
                    OOC_ABSTRACT_MAP_METHOD_PUT, ooc_linkedHashMapPut,
                    OOC_ABSTRACT_MAP_METHOD_REMOVE, ooc_linkedHashMapRemove,
                    OOC_ABSTRACT_MAP_METHOD_CLEAR, ooc_linkedHashMapClear,
                    OOC_ABSTRACT_MAP_METHOD_GET_ITERATOR, ooc_linkedHashMapGetIterator,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &LinkedHashMapClassInstance;
}

void* ooc_linkedHashMapClass(void) {
    if (!LinkedHashMapClass) {
        LinkedHashMapClass = ooc_linkedHashMapClassInit();
    }
    return LinkedHashMapClass;
}
