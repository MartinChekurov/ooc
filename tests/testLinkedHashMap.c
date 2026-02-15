#include "unity.h"
#include "oocLinkedHashMap.h"
#include "oocHashMap.h"
#include "oocHashMapEntry.h"
#include "oocString.h"
#include "oocObject.h"
#include "oocError.h"
#include "oocMap.h"
#include "oocCollection.h"
#include "oocIterable.h"
#include "oocIterator.h"
#include "oocList.h"
#include <stdlib.h>

void test_linked_hash_map_create_destroy(void) {
    void* map = ooc_new(ooc_linkedHashMapClass(), (size_t)0);
    TEST_ASSERT_NOT_NULL(map);
    TEST_ASSERT_TRUE(ooc_isInstanceOf(map, ooc_linkedHashMapClass()));
    ooc_destroy(map);
}

void test_linked_hash_map_insertion_order(void) {
    void* map = ooc_new(ooc_linkedHashMapClass(), (size_t)0);
    void* k1 = ooc_new(ooc_stringClass(), "c");
    void* v1 = ooc_new(ooc_stringClass(), "3");
    void* k2 = ooc_new(ooc_stringClass(), "a");
    void* v2 = ooc_new(ooc_stringClass(), "1");
    void* k3 = ooc_new(ooc_stringClass(), "b");
    void* v3 = ooc_new(ooc_stringClass(), "2");

    ooc_mapPut(map, k1, v1);
    ooc_mapPut(map, k2, v2);
    ooc_mapPut(map, k3, v3);

    /* Iteration should follow insertion order: c, a, b */
    void* it = ooc_mapGetIterator(map);
    void* e1 = ooc_iteratorNext(it);
    void* e2 = ooc_iteratorNext(it);
    void* e3 = ooc_iteratorNext(it);

    TEST_ASSERT_TRUE(ooc_equals(ooc_hashMapEntryGetKey(e1), k1));
    TEST_ASSERT_TRUE(ooc_equals(ooc_hashMapEntryGetKey(e2), k2));
    TEST_ASSERT_TRUE(ooc_equals(ooc_hashMapEntryGetKey(e3), k3));

    ooc_destroy(it);
    ooc_destroy(k1);
    ooc_destroy(v1);
    ooc_destroy(k2);
    ooc_destroy(v2);
    ooc_destroy(k3);
    ooc_destroy(v3);
    ooc_destroy(map);
}

void test_linked_hash_map_update_preserves_order(void) {
    void* map = ooc_new(ooc_linkedHashMapClass(), (size_t)0);
    void* k1 = ooc_new(ooc_stringClass(), "a");
    void* v1 = ooc_new(ooc_stringClass(), "1");
    void* k2 = ooc_new(ooc_stringClass(), "b");
    void* v2 = ooc_new(ooc_stringClass(), "2");
    void* v3 = ooc_new(ooc_stringClass(), "99");

    ooc_mapPut(map, k1, v1);
    ooc_mapPut(map, k2, v2);

    /* Update "a" -> "99", order should still be a, b */
    ooc_mapPut(map, k1, v3);
    TEST_ASSERT_EQUAL(2, ooc_mapSize(map));

    void* it = ooc_mapGetIterator(map);
    void* e1 = ooc_iteratorNext(it);
    void* e2 = ooc_iteratorNext(it);

    TEST_ASSERT_TRUE(ooc_equals(ooc_hashMapEntryGetKey(e1), k1));
    TEST_ASSERT_TRUE(ooc_equals(ooc_hashMapEntryGetValue(e1), v3));
    TEST_ASSERT_TRUE(ooc_equals(ooc_hashMapEntryGetKey(e2), k2));

    ooc_destroy(it);
    ooc_destroy(k1);
    ooc_destroy(v1);
    ooc_destroy(k2);
    ooc_destroy(v2);
    ooc_destroy(v3);
    ooc_destroy(map);
}

void test_linked_hash_map_remove_and_readd(void) {
    void* map = ooc_new(ooc_linkedHashMapClass(), (size_t)0);
    void* k1 = ooc_new(ooc_stringClass(), "a");
    void* v1 = ooc_new(ooc_stringClass(), "1");
    void* k2 = ooc_new(ooc_stringClass(), "b");
    void* v2 = ooc_new(ooc_stringClass(), "2");
    void* k3 = ooc_new(ooc_stringClass(), "c");
    void* v3 = ooc_new(ooc_stringClass(), "3");

    ooc_mapPut(map, k1, v1);
    ooc_mapPut(map, k2, v2);
    ooc_mapPut(map, k3, v3);

    /* Remove "a" and re-add: order becomes b, c, a */
    ooc_mapRemove(map, k1);
    ooc_mapPut(map, k1, v1);

    void* it = ooc_mapGetIterator(map);
    void* e1 = ooc_iteratorNext(it);
    void* e2 = ooc_iteratorNext(it);
    void* e3 = ooc_iteratorNext(it);

    TEST_ASSERT_TRUE(ooc_equals(ooc_hashMapEntryGetKey(e1), k2));
    TEST_ASSERT_TRUE(ooc_equals(ooc_hashMapEntryGetKey(e2), k3));
    TEST_ASSERT_TRUE(ooc_equals(ooc_hashMapEntryGetKey(e3), k1));

    ooc_destroy(it);
    ooc_destroy(k1);
    ooc_destroy(v1);
    ooc_destroy(k2);
    ooc_destroy(v2);
    ooc_destroy(k3);
    ooc_destroy(v3);
    ooc_destroy(map);
}

void test_linked_hash_map_equals_with_hash_map(void) {
    void* lmap = ooc_new(ooc_linkedHashMapClass(), (size_t)0);
    void* hmap = ooc_new(ooc_hashMapClass(), (size_t)0);

    void* k1a = ooc_new(ooc_stringClass(), "a");
    void* v1a = ooc_new(ooc_stringClass(), "1");
    void* k1b = ooc_new(ooc_stringClass(), "a");
    void* v1b = ooc_new(ooc_stringClass(), "1");

    ooc_mapPut(lmap, k1a, v1a);
    ooc_mapPut(hmap, k1b, v1b);

    TEST_ASSERT_TRUE(ooc_equals(lmap, hmap));
    TEST_ASSERT_TRUE(ooc_equals(hmap, lmap));

    ooc_destroy(k1a);
    ooc_destroy(v1a);
    ooc_destroy(k1b);
    ooc_destroy(v1b);
    ooc_destroy(lmap);
    ooc_destroy(hmap);
}

void test_linked_hash_map_clone(void) {
    void* map = ooc_new(ooc_linkedHashMapClass(), (size_t)0);
    void* k1 = ooc_new(ooc_stringClass(), "a");
    void* v1 = ooc_new(ooc_stringClass(), "1");
    void* k2 = ooc_new(ooc_stringClass(), "b");
    void* v2 = ooc_new(ooc_stringClass(), "2");

    ooc_mapPut(map, k1, v1);
    ooc_mapPut(map, k2, v2);

    void* clone = ooc_clone(map);
    TEST_ASSERT_NOT_NULL(clone);
    TEST_ASSERT_TRUE(ooc_equals(map, clone));

    /* Verify insertion order preserved in clone */
    void* it = ooc_mapGetIterator(clone);
    void* e1 = ooc_iteratorNext(it);
    void* e2 = ooc_iteratorNext(it);
    TEST_ASSERT_TRUE(ooc_equals(ooc_hashMapEntryGetKey(e1), k1));
    TEST_ASSERT_TRUE(ooc_equals(ooc_hashMapEntryGetKey(e2), k2));

    ooc_destroy(it);
    ooc_destroy(k1);
    ooc_destroy(v1);
    ooc_destroy(k2);
    ooc_destroy(v2);
    ooc_destroy(map);
    ooc_destroy(clone);
}

void test_linked_hash_map_clear(void) {
    void* map = ooc_new(ooc_linkedHashMapClass(), (size_t)0);
    void* key = ooc_new(ooc_stringClass(), "a");
    void* val = ooc_new(ooc_stringClass(), "1");
    ooc_mapPut(map, key, val);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_mapClear(map));
    TEST_ASSERT_TRUE(ooc_mapIsEmpty(map));

    ooc_destroy(key);
    ooc_destroy(val);
    ooc_destroy(map);
}

void test_linked_hash_map_key_set_order(void) {
    void* map = ooc_new(ooc_linkedHashMapClass(), (size_t)0);
    void* k1 = ooc_new(ooc_stringClass(), "c");
    void* v1 = ooc_new(ooc_stringClass(), "3");
    void* k2 = ooc_new(ooc_stringClass(), "a");
    void* v2 = ooc_new(ooc_stringClass(), "1");

    ooc_mapPut(map, k1, v1);
    ooc_mapPut(map, k2, v2);

    void* keySet = ooc_mapKeySet(map);
    TEST_ASSERT_NOT_NULL(keySet);
    TEST_ASSERT_EQUAL(2, ooc_collectionSize(keySet));
    TEST_ASSERT_TRUE(ooc_collectionContains(keySet, k1));
    TEST_ASSERT_TRUE(ooc_collectionContains(keySet, k2));

    ooc_destroy(keySet);
    ooc_destroy(k1);
    ooc_destroy(v1);
    ooc_destroy(k2);
    ooc_destroy(v2);
    ooc_destroy(map);
}

void test_linked_hash_map_values_order(void) {
    void* map = ooc_new(ooc_linkedHashMapClass(), (size_t)0);
    void* k1 = ooc_new(ooc_stringClass(), "c");
    void* v1 = ooc_new(ooc_stringClass(), "3");
    void* k2 = ooc_new(ooc_stringClass(), "a");
    void* v2 = ooc_new(ooc_stringClass(), "1");

    ooc_mapPut(map, k1, v1);
    ooc_mapPut(map, k2, v2);

    void* values = ooc_mapValues(map);
    TEST_ASSERT_NOT_NULL(values);
    TEST_ASSERT_EQUAL(2, ooc_collectionSize(values));
    TEST_ASSERT_TRUE(ooc_collectionContains(values, v1));
    TEST_ASSERT_TRUE(ooc_collectionContains(values, v2));

    ooc_destroy(values);
    ooc_destroy(k1);
    ooc_destroy(v1);
    ooc_destroy(k2);
    ooc_destroy(v2);
    ooc_destroy(map);
}

void test_linked_hash_map_iterator_remove(void) {
    void* map = ooc_new(ooc_linkedHashMapClass(), (size_t)0);
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

    ooc_destroy(k1);
    ooc_destroy(v1);
    ooc_destroy(k2);
    ooc_destroy(v2);
    ooc_destroy(map);
}
