#include "unity.h"
#include "oocLinkedHashSet.h"
#include "oocHashSet.h"
#include "oocString.h"
#include "oocObject.h"
#include "oocError.h"
#include "oocCollection.h"
#include "oocIterable.h"
#include "oocIterator.h"
#include <stdlib.h>
#include <string.h>

void test_linked_hash_set_create_destroy(void) {
    void* set = ooc_new(ooc_linkedHashSetClass(), (size_t)16);
    TEST_ASSERT_NOT_NULL(set);
    TEST_ASSERT_TRUE(ooc_isInstanceOf(set, ooc_linkedHashSetClass()));
    ooc_destroy(set);
}

void test_linked_hash_set_insertion_order(void) {
    void* set = ooc_new(ooc_linkedHashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "c");
    void* s2 = ooc_new(ooc_stringClass(), "a");
    void* s3 = ooc_new(ooc_stringClass(), "b");

    ooc_collectionAdd(set, s1);
    ooc_collectionAdd(set, s2);
    ooc_collectionAdd(set, s3);

    /* Iteration should follow insertion order: c, a, b */
    void* it = ooc_iterableGetIterator(set);
    void* e1 = ooc_iteratorNext(it);
    void* e2 = ooc_iteratorNext(it);
    void* e3 = ooc_iteratorNext(it);

    TEST_ASSERT_TRUE(ooc_equals(e1, s1));
    TEST_ASSERT_TRUE(ooc_equals(e2, s2));
    TEST_ASSERT_TRUE(ooc_equals(e3, s3));

    ooc_destroy(it);
    ooc_destroy(set);
}

void test_linked_hash_set_no_duplicates(void) {
    void* set = ooc_new(ooc_linkedHashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* dup = ooc_new(ooc_stringClass(), "a");

    ooc_collectionAdd(set, s1);
    ooc_collectionAdd(set, s2);
    ooc_collectionAdd(set, dup);

    TEST_ASSERT_EQUAL(2, ooc_collectionSize(set));

    /* Order should still be a, b (duplicate doesn't change position) */
    void* it = ooc_iterableGetIterator(set);
    void* e1 = ooc_iteratorNext(it);
    void* e2 = ooc_iteratorNext(it);
    TEST_ASSERT_TRUE(ooc_equals(e1, s1));
    TEST_ASSERT_TRUE(ooc_equals(e2, s2));

    ooc_destroy(it);
    ooc_destroy(set);
}

void test_linked_hash_set_remove_and_readd(void) {
    void* set = ooc_new(ooc_linkedHashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");

    ooc_collectionAdd(set, s1);
    ooc_collectionAdd(set, s2);
    ooc_collectionAdd(set, s3);

    /* Remove "a" and re-add: order becomes b, c, a */
    ooc_collectionRemove(set, s1);
    void* s1b = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(set, s1b);

    void* it = ooc_iterableGetIterator(set);
    void* e1 = ooc_iteratorNext(it);
    void* e2 = ooc_iteratorNext(it);
    void* e3 = ooc_iteratorNext(it);

    TEST_ASSERT_TRUE(ooc_equals(e1, s2));
    TEST_ASSERT_TRUE(ooc_equals(e2, s3));
    TEST_ASSERT_TRUE(ooc_equals(e3, s1b));

    ooc_destroy(it);
    ooc_destroy(set);
}

void test_linked_hash_set_equals_with_hash_set(void) {
    void* lset = ooc_new(ooc_linkedHashSetClass(), (size_t)16);
    void* hset = ooc_new(ooc_hashSetClass(), (size_t)16);

    void* s1a = ooc_new(ooc_stringClass(), "a");
    void* s2a = ooc_new(ooc_stringClass(), "b");
    void* s1b = ooc_new(ooc_stringClass(), "a");
    void* s2b = ooc_new(ooc_stringClass(), "b");

    ooc_collectionAdd(lset, s1a);
    ooc_collectionAdd(lset, s2a);
    ooc_collectionAdd(hset, s2b);
    ooc_collectionAdd(hset, s1b);

    TEST_ASSERT_TRUE(ooc_equals(lset, hset));
    TEST_ASSERT_TRUE(ooc_equals(hset, lset));

    ooc_destroy(lset);
    ooc_destroy(hset);
}

void test_linked_hash_set_clone(void) {
    void* set = ooc_new(ooc_linkedHashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(set, s1);
    ooc_collectionAdd(set, s2);

    void* clone = ooc_clone(set);
    TEST_ASSERT_NOT_NULL(clone);
    TEST_ASSERT_TRUE(ooc_equals(set, clone));

    /* Verify insertion order preserved in clone */
    void* it = ooc_iterableGetIterator(clone);
    void* e1 = ooc_iteratorNext(it);
    void* e2 = ooc_iteratorNext(it);
    TEST_ASSERT_TRUE(ooc_equals(e1, s1));
    TEST_ASSERT_TRUE(ooc_equals(e2, s2));

    ooc_destroy(it);
    ooc_destroy(set);
    ooc_destroy(clone);
}

void test_linked_hash_set_to_string(void) {
    void* set = ooc_new(ooc_linkedHashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(set, s1);

    char* str = ooc_toString(set);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_TRUE(str[0] == '{');
    TEST_ASSERT_TRUE(str[strlen(str) - 1] == '}');
    free(str);

    ooc_destroy(set);
}

void test_linked_hash_set_clear(void) {
    void* set = ooc_new(ooc_linkedHashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(set, s1);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_collectionClear(set));
    TEST_ASSERT_TRUE(ooc_collectionIsEmpty(set));

    ooc_destroy(set);
}

void test_linked_hash_set_contains(void) {
    void* set = ooc_new(ooc_linkedHashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");

    ooc_collectionAdd(set, s1);
    TEST_ASSERT_TRUE(ooc_collectionContains(set, s1));
    TEST_ASSERT_FALSE(ooc_collectionContains(set, s2));

    ooc_destroy(set);
}

void test_linked_hash_set_iterator_remove(void) {
    void* set = ooc_new(ooc_linkedHashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(set, s1);
    ooc_collectionAdd(set, s2);

    void* it = ooc_iterableGetIterator(set);
    ooc_iteratorNext(it);
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_iteratorRemove(it));
    ooc_destroy(it);

    TEST_ASSERT_EQUAL(1, ooc_collectionSize(set));

    ooc_destroy(set);
}
