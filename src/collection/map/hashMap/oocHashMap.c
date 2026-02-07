#include "oocHashMap.h"
#include "oocHashMap.r"
#include "oocHashMapEntry.h"
#include "oocHashMapEntry.r"
#include "oocMap.h"
#include "oocMap.r"
#include "oocList.h"
#include "oocArrayList.h"
#include "oocLinkedList.h"
#include "oocHashSet.h"
#include "oocObject.h"
#include "oocObject.r"
#include "oocIterator.h"
#include "oocIterator.r"
#include "oocSet.h"
#include "oocIterable.h"
#include "oocBaseIterator.h"
#include "oocBaseIterator.r"
#include <stdlib.h>
#include <string.h>

#define OOC_HASH_MAP_DEFAULT_CAPACITY 16
#define OOC_HASH_MAP_DEFAULT_LOAD_FACTOR 0.75f

typedef struct OOC_HashMapKeyIterator OOC_HashMapKeyIterator;
typedef struct OOC_HashMapKeyIteratorClass OOC_HashMapKeyIteratorClass;

struct OOC_HashMapKeyIterator {
    OOC_BaseIterator object;
    OOC_HashMap* map;
    void* bucketsIterator;
    void* currentBucketIterator;
};

struct OOC_HashMapKeyIteratorClass {
    OOC_BaseIteratorClass class;
};

static OOC_HashMapClass* HashMapClass;
static OOC_HashMapClass HashMapClassInstance;

static OOC_HashMapKeyIteratorClass* HashMapKeyIteratorClass;
static OOC_HashMapKeyIteratorClass HashMapKeyIteratorClassInstance;

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

static size_t ooc_hashMapSize(void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), 0);
    OOC_HashMap* map = self;
    return map->size;
}

static bool ooc_hashMapIsEmpty(void* self) {
    if (!self) {
        return true;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), true);
    OOC_HashMap* map = self;
    return map->size == 0;
}

static void* ooc_hashMapGetByKey(void* self, void* key) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), NULL);
    OOC_HashMap* map = self;
    void* bucketIterator = ooc_listGetIterator(ooc_hashMapKeyBucket(map, key));
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

static void* ooc_hashMapGetByValue(void* self, void* value) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), NULL);
    OOC_HashMap* map = self;
    void* bucketsIterator = ooc_listGetIterator(map->buckets);
    while (ooc_iteratorHasNext(bucketsIterator)) {
        void* bucket = ooc_iteratorNext(bucketsIterator);
        void* bucketIterator = ooc_listGetIterator(bucket);
        while (ooc_iteratorHasNext(bucketIterator)) {
            void* entry = ooc_iteratorNext(bucketIterator);
            if (ooc_equals(ooc_hashMapEntryGetValue(entry), value)) {
                ooc_destroy(bucketIterator);
                ooc_destroy(bucketsIterator);
                return entry;
            }
        }
        ooc_destroy(bucketIterator);
    }
    ooc_destroy(bucketsIterator);
    return NULL;
}

static bool ooc_hashMapContainsKey(void* self, void* key) {
    return ooc_hashMapGetByKey(self, key) != NULL;
}

static bool ooc_hashMapContainsValue(void* self, void* value) {
    return ooc_hashMapGetByValue(self, value) != NULL;
}

static OOC_Error ooc_hashMapPut(void* self, void* key, void* value) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMap* map = self;
    void* existingEntry = ooc_hashMapGetByKey(map, key);
    if (existingEntry) {
        ooc_destroy(ooc_hashMapEntryGetKey(existingEntry));
        ooc_destroy(ooc_hashMapEntryGetValue(existingEntry));
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

static OOC_Error ooc_hashMapRemove(void* self, void* key) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMap* map = self;
    void* bucket = ooc_hashMapKeyBucket(map, key);
    void* bucketIterator = ooc_listGetIterator(bucket);
    while (ooc_iteratorHasNext(bucketIterator)) {
        void* entry = ooc_iteratorNext(bucketIterator);
        if (ooc_equals(ooc_hashMapEntryGetKey(entry), key)) {
            ooc_iteratorRemove(bucketIterator);
            map->size--;
            ooc_destroy(bucketIterator);
            return OOC_ERROR_NONE;
        }
    }
    ooc_destroy(bucketIterator);
    return OOC_ERROR_NOT_FOUND;
}

static OOC_Error ooc_hashMapClear(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMap* map = self;
    size_t count = 0;
    void* bucketsIterator = ooc_listGetIterator(map->buckets);
    while (ooc_iteratorHasNext(bucketsIterator)) {
        ooc_destroy(ooc_iteratorNext(bucketsIterator));
        ooc_listSetAt(map->buckets, count++, NULL);
    }
    ooc_destroy(bucketsIterator);
    map->size = 0;
    return OOC_ERROR_NONE;
}

static void* ooc_hashMapKeySet(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), NULL);
    OOC_HashMap* map = self;
    void* keySet = ooc_new(ooc_hashSetClass(), map->size);
    if (!keySet) {
        return NULL;
    }
    void* bucketsIterator = ooc_listGetIterator(map->buckets);
    while (ooc_iteratorHasNext(bucketsIterator)) {
        void* bucket = ooc_iteratorNext(bucketsIterator);
        void* bucketIterator = ooc_listGetIterator(bucket);
        while (ooc_iteratorHasNext(bucketIterator)) {
            void* entry = ooc_iteratorNext(bucketIterator);
            ooc_setAdd(keySet, ooc_hashMapEntryGetKey(entry));
        }
        ooc_destroy(bucketIterator);
    }
    ooc_destroy(bucketsIterator);
    return keySet;
}

static void* ooc_hashMapValues(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), NULL);
    OOC_HashMap* map = self;
    void* values = ooc_new(ooc_arrayListClass(), map->size);
    if (!values) {
        return NULL;
    }
    void* bucketsIterator = ooc_listGetIterator(map->buckets);
    while (ooc_iteratorHasNext(bucketsIterator)) {
        void* bucket = ooc_iteratorNext(bucketsIterator);
        void* bucketIterator = ooc_listGetIterator(bucket);
        while (ooc_iteratorHasNext(bucketIterator)) {
            void* entry = ooc_iteratorNext(bucketIterator);
            ooc_listAdd(values, ooc_hashMapEntryGetValue(entry));
        }
        ooc_destroy(bucketIterator);
    }
    ooc_destroy(bucketsIterator);
    return values;
}

static void advanceToNextNonEmptyBucket(OOC_HashMapKeyIterator* iterator) {
    while (ooc_iteratorHasNext(iterator->bucketsIterator)) {
        void* bucket = ooc_iteratorNext(iterator->bucketsIterator);
        if (ooc_listSize(bucket)) {
            iterator->currentBucketIterator = ooc_iterableGetIterator(bucket);
            break;
        }
    }
}

static bool ooc_hashSetIteratorHasNext(void* self) {
    if (!self) {
        return false;
    }
    OOC_TYPE_CHECK(self, HashMapKeyIteratorClass, false);
    OOC_HashMapKeyIterator* iterator = self;
    return ooc_iteratorHasNext(iterator->currentBucketIterator);
}

static void* ooc_hashSetIteratorNext(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, HashMapKeyIteratorClass, NULL);
    OOC_HashMapKeyIterator* iterator = self;
    ooc_superBaseIteratorNext(iterator);
    void* element = ooc_iteratorNext(iterator->currentBucketIterator);
    if (!ooc_iteratorHasNext(iterator->currentBucketIterator)) {
        ooc_destroy(iterator->currentBucketIterator);
        iterator->currentBucketIterator = NULL;
        advanceToNextNonEmptyBucket(iterator);
    }
    return element;
}

static OOC_Error ooc_hashSetIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, HashMapKeyIteratorClass, OOC_ERROR_INVALID_OBJECT);
    OOC_HashMapKeyIterator* iterator = self;
    OOC_Error error = ooc_superCtor(iterator, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    void* iteratorInterface = ooc_new(ooc_iteratorClass(),
                                      OOC_ITERATOR_METHOD_HAS_NEXT, ooc_hashSetIteratorHasNext,
                                      OOC_ITERATOR_METHOD_NEXT, ooc_hashSetIteratorNext,
                                      0);
    error = ooc_addInterface(iterator, iteratorInterface);
    if (error != OOC_ERROR_NONE) {
        ooc_destroy(iteratorInterface);
        return error;
    }
    iterator->map = va_arg(*args, OOC_HashMap*);
    if (!iterator->map) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    iterator->currentBucketIterator = NULL;
    iterator->bucketsIterator = ooc_listGetIterator(iterator->map->buckets);
    advanceToNextNonEmptyBucket(iterator);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_hashSetIteratorDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, HashMapKeyIteratorClass, OOC_ERROR_INVALID_OBJECT);
    OOC_HashMapKeyIterator* iterator = self;
    ooc_destroy(iterator->bucketsIterator);
    iterator->bucketsIterator = NULL;
    ooc_destroy(iterator->currentBucketIterator);
    iterator->currentBucketIterator = NULL;
    return ooc_superDtor(self);
}

static void* ooc_hashMapKeyIteratorClassInit(void) {
    if (ooc_classNew(&HashMapKeyIteratorClassInstance,
                    "HashMapKeyIterator",
                    sizeof(OOC_HashMapKeyIterator),
                    sizeof(OOC_HashMapKeyIteratorClass),
                    ooc_baseIteratorClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_hashSetIteratorCtor,
                    OOC_METHOD_DTOR, ooc_hashSetIteratorDtor,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &HashMapKeyIteratorClassInstance;
}

void* ooc_hashMapKeyIteratorClass(void) {
    if (!HashMapKeyIteratorClass) {
        HashMapKeyIteratorClass = ooc_hashMapKeyIteratorClassInit();
    }
    return HashMapKeyIteratorClass;
}

void* ooc_hashMapGetKeyIterator(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), NULL);
    return ooc_new(ooc_hashMapKeyIteratorClass(), self);
}

static OOC_Error ooc_hashMapCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMap* map = self;
    OOC_Error error = ooc_superCtor(map, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    void* mapInterface = ooc_new(ooc_mapClass(),
                              OOC_MAP_METHOD_SIZE, ooc_hashMapSize,
                              OOC_MAP_METHOD_IS_EMPTY, ooc_hashMapIsEmpty,
                              OOC_MAP_METHOD_CONTAINS_KEY, ooc_hashMapContainsKey,
                              OOC_MAP_METHOD_CONTAINS_VALUE, ooc_hashMapContainsValue,
                              OOC_MAP_METHOD_GET, ooc_hashMapGetByKey,
                              OOC_MAP_METHOD_PUT, ooc_hashMapPut,
                              OOC_MAP_METHOD_REMOVE, ooc_hashMapRemove,
                              OOC_MAP_METHOD_CLEAR, ooc_hashMapClear,
                              OOC_MAP_METHOD_KEY_SET, ooc_hashMapKeySet,
                              OOC_MAP_METHOD_VALUES, ooc_hashMapValues,
                              0);
    error = ooc_addInterface(map, mapInterface);
    if (error != OOC_ERROR_NONE) {
        ooc_destroy(mapInterface);
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
    return ooc_superDtor(map);
}

static void* ooc_hashMapClassInit(void) {
    if (ooc_classNew(&HashMapClassInstance,
                    "HashMap",
                    sizeof(OOC_HashMap),
                    sizeof(OOC_HashMapClass),
                    ooc_objectClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_hashMapCtor,
                    OOC_METHOD_DTOR, ooc_hashMapDtor,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &HashMapClassInstance;
}

void* ooc_hashMapClass() {
    if (!HashMapClass) {
        HashMapClass = ooc_hashMapClassInit();
    }
    return HashMapClass;
}