#include "unity.h"
#include "oocHashMap.h"
#include "oocHashMapEntry.h"
#include "oocString.h"
#include "oocObject.h"
#include "oocError.h"
#include "oocMap.h"
#include "oocCollection.h"
#include "oocIterator.h"
#include <stdlib.h>
#include <string.h>

void test_hash_map_create_destroy(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    TEST_ASSERT_NOT_NULL(map);
    TEST_ASSERT_TRUE(ooc_isInstanceOf(map, ooc_hashMapClass()));
    ooc_destroy(map);
}

void test_hash_map_put_get_single(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* key = ooc_new(ooc_stringClass(), "key1");
    void* val = ooc_new(ooc_stringClass(), "val1");

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_mapPut(map, key, val));
    TEST_ASSERT_EQUAL(1, ooc_mapSize(map));

    void* result = ooc_mapGet(map, key);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_TRUE(ooc_equals(result, val));

    ooc_destroy(map);
}

void test_hash_map_put_get_multiple(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* k1 = ooc_new(ooc_stringClass(), "a");
    void* v1 = ooc_new(ooc_stringClass(), "1");
    void* k2 = ooc_new(ooc_stringClass(), "b");
    void* v2 = ooc_new(ooc_stringClass(), "2");
    void* k3 = ooc_new(ooc_stringClass(), "c");
    void* v3 = ooc_new(ooc_stringClass(), "3");

    ooc_mapPut(map, k1, v1);
    ooc_mapPut(map, k2, v2);
    ooc_mapPut(map, k3, v3);

    TEST_ASSERT_EQUAL(3, ooc_mapSize(map));
    TEST_ASSERT_TRUE(ooc_equals(ooc_mapGet(map, k1), v1));
    TEST_ASSERT_TRUE(ooc_equals(ooc_mapGet(map, k2), v2));
    TEST_ASSERT_TRUE(ooc_equals(ooc_mapGet(map, k3), v3));

    ooc_destroy(map);
}

void test_hash_map_put_overwrite(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* key = ooc_new(ooc_stringClass(), "key");
    void* v1 = ooc_new(ooc_stringClass(), "old");
    void* v2 = ooc_new(ooc_stringClass(), "new");

    ooc_mapPut(map, key, v1);
    ooc_mapPut(map, key, v2);

    TEST_ASSERT_EQUAL(1, ooc_mapSize(map));
    TEST_ASSERT_TRUE(ooc_equals(ooc_mapGet(map, key), v2));

    ooc_destroy(map);
}

void test_hash_map_contains_key(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* key = ooc_new(ooc_stringClass(), "key");
    void* val = ooc_new(ooc_stringClass(), "val");
    void* other = ooc_new(ooc_stringClass(), "other");

    ooc_mapPut(map, key, val);

    TEST_ASSERT_TRUE(ooc_mapContainsKey(map, key));
    TEST_ASSERT_FALSE(ooc_mapContainsKey(map, other));

    ooc_destroy(other);
    ooc_destroy(map);
}

void test_hash_map_contains_value(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* key = ooc_new(ooc_stringClass(), "key");
    void* val = ooc_new(ooc_stringClass(), "val");
    void* other = ooc_new(ooc_stringClass(), "other");

    ooc_mapPut(map, key, val);

    TEST_ASSERT_TRUE(ooc_mapContainsValue(map, val));
    TEST_ASSERT_FALSE(ooc_mapContainsValue(map, other));

    ooc_destroy(other);
    ooc_destroy(map);
}

void test_hash_map_remove(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* k1 = ooc_new(ooc_stringClass(), "a");
    void* v1 = ooc_new(ooc_stringClass(), "1");
    void* k2 = ooc_new(ooc_stringClass(), "b");
    void* v2 = ooc_new(ooc_stringClass(), "2");

    ooc_mapPut(map, k1, v1);
    ooc_mapPut(map, k2, v2);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_mapRemove(map, k1));
    TEST_ASSERT_EQUAL(1, ooc_mapSize(map));

    void* removedProbe = ooc_new(ooc_stringClass(), "a");
    TEST_ASSERT_FALSE(ooc_mapContainsKey(map, removedProbe));
    ooc_destroy(removedProbe);

    TEST_ASSERT_TRUE(ooc_mapContainsKey(map, k2));

    ooc_destroy(map);
}

void test_hash_map_size_and_empty(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    TEST_ASSERT_TRUE(ooc_mapIsEmpty(map));
    TEST_ASSERT_EQUAL(0, ooc_mapSize(map));

    void* key = ooc_new(ooc_stringClass(), "a");
    void* val = ooc_new(ooc_stringClass(), "1");
    ooc_mapPut(map, key, val);
    TEST_ASSERT_FALSE(ooc_mapIsEmpty(map));
    TEST_ASSERT_EQUAL(1, ooc_mapSize(map));

    ooc_destroy(map);
}

void test_hash_map_clear(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* key = ooc_new(ooc_stringClass(), "a");
    void* val = ooc_new(ooc_stringClass(), "1");
    ooc_mapPut(map, key, val);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_mapClear(map));
    TEST_ASSERT_EQUAL(0, ooc_mapSize(map));
    TEST_ASSERT_TRUE(ooc_mapIsEmpty(map));

    ooc_destroy(map);
}

void test_hash_map_iterator(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* k1 = ooc_new(ooc_stringClass(), "a");
    void* v1 = ooc_new(ooc_stringClass(), "1");
    void* k2 = ooc_new(ooc_stringClass(), "b");
    void* v2 = ooc_new(ooc_stringClass(), "2");

    ooc_mapPut(map, k1, v1);
    ooc_mapPut(map, k2, v2);

    void* it = ooc_mapGetIterator(map);
    TEST_ASSERT_NOT_NULL(it);

    int count = 0;
    while (ooc_iteratorHasNext(it)) {
        void* entry = ooc_iteratorNext(it);
        TEST_ASSERT_NOT_NULL(entry);
        void* key = ooc_hashMapEntryGetKey(entry);
        void* value = ooc_hashMapEntryGetValue(entry);
        TEST_ASSERT_NOT_NULL(key);
        count++;
    }
    TEST_ASSERT_EQUAL(2, count);

    ooc_destroy(it);
    ooc_destroy(map);
}

void test_hash_map_iterator_remove(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* k1 = ooc_new(ooc_stringClass(), "a");
    void* v1 = ooc_new(ooc_stringClass(), "1");
    void* k2 = ooc_new(ooc_stringClass(), "b");
    void* v2 = ooc_new(ooc_stringClass(), "2");

    ooc_mapPut(map, k1, v1);
    ooc_mapPut(map, k2, v2);

    void* it = ooc_mapGetIterator(map);
    ooc_iteratorNext(it);
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_iteratorRemove(it));
    ooc_destroy(it);

    TEST_ASSERT_EQUAL(1, ooc_mapSize(map));

    ooc_destroy(map);
}

void test_hash_map_key_set(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* k1 = ooc_new(ooc_stringClass(), "a");
    void* v1 = ooc_new(ooc_stringClass(), "1");
    void* k2 = ooc_new(ooc_stringClass(), "b");
    void* v2 = ooc_new(ooc_stringClass(), "2");

    ooc_mapPut(map, k1, v1);
    ooc_mapPut(map, k2, v2);

    void* keySet = ooc_mapKeySet(map);
    TEST_ASSERT_NOT_NULL(keySet);
    TEST_ASSERT_EQUAL(2, ooc_collectionSize(keySet));
    TEST_ASSERT_TRUE(ooc_collectionContains(keySet, k1));
    TEST_ASSERT_TRUE(ooc_collectionContains(keySet, k2));

    ooc_destroy(keySet);
    ooc_destroy(map);
}

void test_hash_map_values(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* k1 = ooc_new(ooc_stringClass(), "a");
    void* v1 = ooc_new(ooc_stringClass(), "1");
    void* k2 = ooc_new(ooc_stringClass(), "b");
    void* v2 = ooc_new(ooc_stringClass(), "2");

    ooc_mapPut(map, k1, v1);
    ooc_mapPut(map, k2, v2);

    void* values = ooc_mapValues(map);
    TEST_ASSERT_NOT_NULL(values);
    TEST_ASSERT_EQUAL(2, ooc_collectionSize(values));
    TEST_ASSERT_TRUE(ooc_collectionContains(values, v1));
    TEST_ASSERT_TRUE(ooc_collectionContains(values, v2));

    ooc_destroy(values);
    ooc_destroy(map);
}

void test_hash_map_equals(void) {
    void* map1 = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* map2 = ooc_new(ooc_hashMapClass(), (size_t)16);

    void* k1a = ooc_new(ooc_stringClass(), "a");
    void* v1a = ooc_new(ooc_stringClass(), "1");
    void* k1b = ooc_new(ooc_stringClass(), "a");
    void* v1b = ooc_new(ooc_stringClass(), "1");

    ooc_mapPut(map1, k1a, v1a);
    ooc_mapPut(map2, k1b, v1b);

    TEST_ASSERT_TRUE(ooc_equals(map1, map2));

    void* k2 = ooc_new(ooc_stringClass(), "b");
    void* v2 = ooc_new(ooc_stringClass(), "2");
    ooc_mapPut(map2, k2, v2);
    TEST_ASSERT_FALSE(ooc_equals(map1, map2));

    ooc_destroy(map1);
    ooc_destroy(map2);
}

void test_hash_map_hash_code(void) {
    void* map1 = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* map2 = ooc_new(ooc_hashMapClass(), (size_t)16);

    void* k1a = ooc_new(ooc_stringClass(), "a");
    void* v1a = ooc_new(ooc_stringClass(), "1");
    void* k1b = ooc_new(ooc_stringClass(), "a");
    void* v1b = ooc_new(ooc_stringClass(), "1");

    ooc_mapPut(map1, k1a, v1a);
    ooc_mapPut(map2, k1b, v1b);

    TEST_ASSERT_EQUAL(ooc_hashCode(map1), ooc_hashCode(map2));

    ooc_destroy(map1);
    ooc_destroy(map2);
}

void test_hash_map_to_string(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* key = ooc_new(ooc_stringClass(), "a");
    void* val = ooc_new(ooc_stringClass(), "1");
    ooc_mapPut(map, key, val);

    char* str = ooc_toString(map);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_TRUE(str[0] == '{');
    TEST_ASSERT_TRUE(str[strlen(str) - 1] == '}');
    /* Should contain key=value format */
    TEST_ASSERT_NOT_NULL(strstr(str, "="));
    free(str);

    ooc_destroy(map);
}

void test_hash_map_clone(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)16);
    void* k1 = ooc_new(ooc_stringClass(), "a");
    void* v1 = ooc_new(ooc_stringClass(), "1");
    void* k2 = ooc_new(ooc_stringClass(), "b");
    void* v2 = ooc_new(ooc_stringClass(), "2");

    ooc_mapPut(map, k1, v1);
    ooc_mapPut(map, k2, v2);

    void* clone = ooc_clone(map);
    TEST_ASSERT_NOT_NULL(clone);
    TEST_ASSERT_TRUE(ooc_equals(map, clone));
    TEST_ASSERT_EQUAL(ooc_mapSize(map), ooc_mapSize(clone));

    /* Verify independence */
    void* k3 = ooc_new(ooc_stringClass(), "c");
    void* v3 = ooc_new(ooc_stringClass(), "3");
    ooc_mapPut(clone, k3, v3);
    TEST_ASSERT_EQUAL(2, ooc_mapSize(map));
    TEST_ASSERT_EQUAL(3, ooc_mapSize(clone));

    ooc_destroy(map);
    ooc_destroy(clone);
}

void test_hash_map_supports_null_key_and_value(void) {
    void* map = ooc_new(ooc_hashMapClass(), (size_t)0);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_mapPut(map, NULL, NULL));
    TEST_ASSERT_EQUAL(1, ooc_mapSize(map));
    TEST_ASSERT_TRUE(ooc_mapContainsKey(map, NULL));
    TEST_ASSERT_TRUE(ooc_mapContainsValue(map, NULL));
    TEST_ASSERT_NULL(ooc_mapGet(map, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_mapRemove(map, NULL));
    TEST_ASSERT_TRUE(ooc_mapIsEmpty(map));

    ooc_destroy(map);
}
