#include "oocHashMap.h"
#include "oocHashMap.r"
#include "oocHashMapEntry.h"
#include "oocAbstractMap.h"
#include "oocAbstractMap.r"
#include "oocMap.h"
#include "oocList.h"
#include "oocArrayList.h"
#include "oocLinkedList.h"
#include "oocObject.h"
#include "oocObject.r"
#include "oocIterator.h"
#include "oocIterable.h"
#include "oocAbstractIterator.h"
#include "oocAbstractIterator.r"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define OOC_HASH_MAP_DEFAULT_CAPACITY 16
#define OOC_HASH_MAP_DEFAULT_LOAD_FACTOR 0.75f

typedef struct OOC_HashMapIterator OOC_HashMapIterator;
typedef struct OOC_HashMapIteratorClass OOC_HashMapIteratorClass;

struct OOC_HashMapIterator {
    OOC_AbstractIterator object;
    OOC_HashMap* map;
    void* bucketsIterator;
    void* currentBucketIterator;
};

struct OOC_HashMapIteratorClass {
    OOC_AbstractIteratorClass class;
};

static OOC_HashMapClass* HashMapClass;
static OOC_HashMapClass HashMapClassInstance;

static OOC_HashMapIteratorClass* HashMapIteratorClass;
static OOC_HashMapIteratorClass HashMapIteratorClassInstance;

void* ooc_hashMapIteratorClass(void);

static size_t ooc_hashMapHashFunction(void* key, size_t capacity) {
    if (!key) {
        return 0;
    }
    size_t hash = ooc_hashCode(key);
    return hash % capacity;
}

static size_t ooc_hashMapCapacity(OOC_HashMap* map) {
    if (!map || !map->buckets) {
        return 0;
    }
    return ooc_listSize(map->buckets);
}

static size_t ooc_hashMapKeyIndex(OOC_HashMap* map, void* key) {
    if (!map || !key) {
        return 0;
    }
    return ooc_hashMapHashFunction(key, ooc_hashMapCapacity(map));
}

static void* ooc_hashMapKeyBucket(OOC_HashMap* map, void* key) {
    if (!map || !key) {
        return NULL;
    }
    return ooc_listGetAt(map->buckets, ooc_hashMapKeyIndex(map, key));
}

static bool ooc_hashMapRehash(OOC_HashMap* map) {
    if (!map) {
        return false;
    }
    OOC_TYPE_CHECK(map, ooc_hashMapClass(), false);
    size_t oldCapacity = ooc_hashMapCapacity(map);
    void* oldBuckets = map->buckets;
    size_t newCapacity = oldCapacity * 2;
    map->buckets = ooc_new(ooc_arrayListClass(), newCapacity);
    if (!map->buckets) {
        map->buckets = oldBuckets;
        return false;
    }
    map->size = 0;
    void* bucketsIterator = ooc_listGetIterator(oldBuckets);
    while (ooc_iteratorHasNext(bucketsIterator)) {
        void* bucket = ooc_iteratorNext(bucketsIterator);
        void* bucketIterator = ooc_listGetIterator(bucket);
        while (ooc_iteratorHasNext(bucketIterator)) {
            void* entry = ooc_iteratorNext(bucketIterator);
            size_t newIndex = ooc_hashMapHashFunction(ooc_hashMapEntryGetKey(entry), newCapacity);
            void* newBucket = ooc_listGetAt(map->buckets, newIndex);
            if (!newBucket) {
                newBucket = ooc_new(ooc_linkedListClass());
                if (!newBucket) {
                    ooc_destroy(bucketIterator);
                    ooc_destroy(bucketsIterator);
                    ooc_destroy(map->buckets);
                    map->buckets = oldBuckets;
                    return false;
                }
                ooc_listSetAt(map->buckets, newIndex, newBucket);
            }
            ooc_listAdd(newBucket, entry);
            map->size++;
        }
        ooc_destroy(bucketIterator);
    }
    ooc_destroy(bucketsIterator);
    ooc_destroy(oldBuckets);
    return true;
}

static void* ooc_hashMapGetByKey(void* self, void* key) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), NULL);
    OOC_HashMap* map = self;
    void* bucket = ooc_hashMapKeyBucket(map, key);
    if (!bucket) {
        return NULL;
    }
    void* bucketIterator = ooc_listGetIterator(bucket);
    while (ooc_iteratorHasNext(bucketIterator)) {
        void* entry = ooc_iteratorNext(bucketIterator);
        if (ooc_equals(ooc_hashMapEntryGetKey(entry), key)) {
            ooc_destroy(bucketIterator);
            return entry;
        }
    }
    ooc_destroy(bucketIterator);
    return NULL;
}

size_t ooc_hashMapSize(void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), 0);
    OOC_HashMap* map = self;
    return map->size;
}

bool ooc_hashMapIsEmpty(void* self) {
    return ooc_mapIsEmpty(self);
}

bool ooc_hashMapContainsKey(void* self, void* key) {
    return ooc_mapContainsKey(self, key);
}

bool ooc_hashMapContainsValue(void* self, void* value) {
    return ooc_mapContainsValue(self, value);
}

void* ooc_hashMapGet(void* self, void* key) {
    return ooc_mapGet(self, key);
}

OOC_Error ooc_hashMapPut(void* self, void* key, void* value) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMap* map = self;
    void* existingEntry = ooc_hashMapGetByKey(map, key);
    if (existingEntry) {
        ooc_hashMapEntrySetKey(existingEntry, key);
        ooc_hashMapEntrySetValue(existingEntry, value);
        return OOC_ERROR_NONE;
    }
    size_t capacity = ooc_hashMapCapacity(map);
    if ((float)(map->size + 1) / capacity > map->loadFactor) {
        if (!ooc_hashMapRehash(map)) {
            return OOC_ERROR_OUT_OF_MEMORY;
        }
        capacity = ooc_hashMapCapacity(map);
    }
    void* entry = ooc_new(ooc_hashMapEntryClass(), NULL, NULL);
    if (!entry) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    size_t index = ooc_hashMapHashFunction(key, capacity);
    void* bucket = ooc_listGetAt(map->buckets, index);
    if (!bucket) {
        bucket = ooc_new(ooc_linkedListClass());
        if (!bucket) {
            ooc_destroy(entry);
            return OOC_ERROR_OUT_OF_MEMORY;
        }
        ooc_listSetAt(map->buckets, index, bucket);
    }
    OOC_Error error = ooc_listAdd(bucket, entry);
    if (error == OOC_ERROR_NONE) {
        ooc_hashMapEntrySetKey(entry, key);
        ooc_hashMapEntrySetValue(entry, value);
        map->size++;
    } else {
        ooc_destroy(entry);
    }
    return error;
}

OOC_Error ooc_hashMapRemove(void* self, void* key) {
    return ooc_mapRemove(self, key);
}

OOC_Error ooc_hashMapClear(void* self) {
    return ooc_mapClear(self);
}

void* ooc_hashMapKeySet(void* self) {
    return ooc_mapKeySet(self);
}

void* ooc_hashMapValues(void* self) {
    return ooc_mapValues(self);
}

static bool ooc_hashMapIteratorHasNext(void* self) {
    if (!self) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapIteratorClass(), false);
    OOC_HashMapIterator* iterator = self;
    while (!iterator->currentBucketIterator ||
           !ooc_iteratorHasNext(iterator->currentBucketIterator)) {
        if (iterator->currentBucketIterator) {
            ooc_destroy(iterator->currentBucketIterator);
            iterator->currentBucketIterator = NULL;
        }
        if (!ooc_iteratorHasNext(iterator->bucketsIterator)) {
            return false;
        }
        void* bucket = ooc_iteratorNext(iterator->bucketsIterator);
        if (ooc_listSize(bucket) > 0) {
            iterator->currentBucketIterator = ooc_iterableGetIterator(bucket);
        }
    }
    return true;
}

static void* ooc_hashMapIteratorNext(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapIteratorClass(), NULL);
    OOC_HashMapIterator* iterator = self;
    ooc_abstractIteratorNext(iterator);
    return ooc_iteratorNext(iterator->currentBucketIterator);
}

static OOC_Error ooc_hashMapIteratorRemove(void* self) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMapIterator* iterator = self;
    OOC_Error error = ooc_abstractIteratorRemove(iterator);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    error = ooc_iteratorRemove(iterator->currentBucketIterator);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    iterator->map->size--;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_hashMapIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMapIterator* iterator = self;
    OOC_Error error = ooc_superCtor(ooc_hashMapIteratorClass(), iterator, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    iterator->map = va_arg(*args, OOC_HashMap*);
    if (!iterator->map) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    iterator->currentBucketIterator = NULL;
    iterator->bucketsIterator = ooc_listGetIterator(iterator->map->buckets);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_hashMapIteratorDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMapIterator* iterator = self;
    ooc_destroy(iterator->bucketsIterator);
    iterator->bucketsIterator = NULL;
    ooc_destroy(iterator->currentBucketIterator);
    iterator->currentBucketIterator = NULL;
    return ooc_superDtor(ooc_hashMapIteratorClass(), self);
}

static void* ooc_hashMapIteratorClassInit(void) {
    if (ooc_classNew(&HashMapIteratorClassInstance,
                    "HashMapIterator",
                    sizeof(OOC_HashMapIterator),
                    sizeof(OOC_HashMapIteratorClass),
                    ooc_abstractIteratorClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_hashMapIteratorCtor,
                    OOC_METHOD_DTOR, ooc_hashMapIteratorDtor,
                    OOC_ABSTRACT_ITERATOR_METHOD_HAS_NEXT, ooc_hashMapIteratorHasNext,
                    OOC_ABSTRACT_ITERATOR_METHOD_NEXT, ooc_hashMapIteratorNext,
                    OOC_ABSTRACT_ITERATOR_METHOD_REMOVE, ooc_hashMapIteratorRemove,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &HashMapIteratorClassInstance;
}

void* ooc_hashMapIteratorClass(void) {
    if (!HashMapIteratorClass) {
        HashMapIteratorClass = ooc_hashMapIteratorClassInit();
    }
    return HashMapIteratorClass;
}

void* ooc_hashMapGetIterator(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), NULL);
    return ooc_new(ooc_hashMapIteratorClass(), self);
}

static OOC_Error ooc_hashMapCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMap* map = self;
    OOC_Error error = ooc_superCtor(ooc_hashMapClass(), map, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    size_t initialCapacity = va_arg(*args, size_t);
    if (!initialCapacity) {
        initialCapacity = OOC_HASH_MAP_DEFAULT_CAPACITY;
    }
    map->size = 0;
    map->loadFactor = OOC_HASH_MAP_DEFAULT_LOAD_FACTOR;
    map->buckets = ooc_new(ooc_arrayListClass(), initialCapacity);
    if (!map->buckets) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_hashMapDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMap* map = self;
    ooc_destroy(map->buckets);
    map->buckets = NULL;
    map->size = 0;
    map->loadFactor = 0;
    return ooc_superDtor(ooc_hashMapClass(), map);
}

static void* ooc_hashMapClassInit(void) {
    if (ooc_classNew(&HashMapClassInstance,
                    "HashMap",
                    sizeof(OOC_HashMap),
                    sizeof(OOC_HashMapClass),
                    ooc_abstractMapClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_hashMapCtor,
                    OOC_METHOD_DTOR, ooc_hashMapDtor,
                    OOC_ABSTRACT_MAP_METHOD_SIZE, ooc_hashMapSize,
                    OOC_ABSTRACT_MAP_METHOD_PUT, ooc_hashMapPut,
                    OOC_ABSTRACT_MAP_METHOD_GET_ITERATOR, ooc_hashMapGetIterator,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &HashMapClassInstance;
}

void* ooc_hashMapClass(void) {
    if (!HashMapClass) {
        HashMapClass = ooc_hashMapClassInit();
    }
    return HashMapClass;
}
