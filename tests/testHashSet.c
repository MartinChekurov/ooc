#include "unity.h"
#include "oocHashSet.h"
#include "oocString.h"
#include "oocObject.h"
#include "oocError.h"
#include "oocCollection.h"
#include "oocSet.h"
#include "oocIterable.h"
#include "oocIterator.h"
#include <stdlib.h>
#include <string.h>

void test_hash_set_create_destroy(void) {
    void* set = ooc_new(ooc_hashSetClass(), (size_t)16);
    TEST_ASSERT_NOT_NULL(set);
    TEST_ASSERT_TRUE(ooc_isInstanceOf(set, ooc_hashSetClass()));
    ooc_destroy(set);
}

void test_hash_set_add_and_size(void) {
    void* set = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");

    ooc_collectionAdd(set, s1);
    ooc_collectionAdd(set, s2);
    ooc_collectionAdd(set, s3);
    TEST_ASSERT_EQUAL(3, ooc_collectionSize(set));

    ooc_destroy(set);
}

void test_hash_set_no_duplicates(void) {
    void* set = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "a");

    ooc_collectionAdd(set, s1);
    ooc_collectionAdd(set, s2);
    TEST_ASSERT_EQUAL(1, ooc_collectionSize(set));

    ooc_destroy(set);
}

void test_hash_set_contains(void) {
    void* set = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");

    ooc_collectionAdd(set, s1);

    void* lookup = ooc_new(ooc_stringClass(), "a");
    TEST_ASSERT_TRUE(ooc_collectionContains(set, lookup));
    TEST_ASSERT_FALSE(ooc_collectionContains(set, s2));

    ooc_destroy(lookup);
    ooc_destroy(s2);
    ooc_destroy(set);
}

void test_hash_set_remove(void) {
    void* set = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");

    ooc_collectionAdd(set, s1);
    ooc_collectionAdd(set, s2);
    TEST_ASSERT_EQUAL(2, ooc_collectionSize(set));

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_collectionRemove(set, s1));
    TEST_ASSERT_EQUAL(1, ooc_collectionSize(set));
    void* removedProbe = ooc_new(ooc_stringClass(), "a");
    TEST_ASSERT_FALSE(ooc_collectionContains(set, removedProbe));
    ooc_destroy(removedProbe);
    TEST_ASSERT_TRUE(ooc_collectionContains(set, s2));

    ooc_destroy(set);
}

void test_hash_set_clear(void) {
    void* set = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(set, s1);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_collectionClear(set));
    TEST_ASSERT_EQUAL(0, ooc_collectionSize(set));
    TEST_ASSERT_TRUE(ooc_collectionIsEmpty(set));

    ooc_destroy(set);
}

void test_hash_set_iterator(void) {
    void* set = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(set, s1);
    ooc_collectionAdd(set, s2);

    void* it = ooc_iterableGetIterator(set);
    TEST_ASSERT_NOT_NULL(it);

    int count = 0;
    while (ooc_iteratorHasNext(it)) {
        void* elem = ooc_iteratorNext(it);
        TEST_ASSERT_NOT_NULL(elem);
        /* Elements should be String objects (keys), not map entries */
        TEST_ASSERT_TRUE(ooc_isInstanceOf(elem, ooc_stringClass()));
        count++;
    }
    TEST_ASSERT_EQUAL(2, count);

    ooc_destroy(it);
    ooc_destroy(set);
}

void test_hash_set_iterator_remove(void) {
    void* set = ooc_new(ooc_hashSetClass(), (size_t)16);
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

void test_hash_set_contains_all(void) {
    void* set1 = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* set2 = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");
    void* t1 = ooc_new(ooc_stringClass(), "a");
    void* t2 = ooc_new(ooc_stringClass(), "b");

    ooc_collectionAdd(set1, s1);
    ooc_collectionAdd(set1, s2);
    ooc_collectionAdd(set1, s3);
    ooc_collectionAdd(set2, t1);
    ooc_collectionAdd(set2, t2);

    TEST_ASSERT_TRUE(ooc_collectionContainsAll(set1, set2));
    TEST_ASSERT_FALSE(ooc_collectionContainsAll(set2, set1));

    ooc_destroy(set1);
    ooc_destroy(set2);
}

void test_hash_set_equals(void) {
    void* set1 = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* set2 = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* s1a = ooc_new(ooc_stringClass(), "a");
    void* s2a = ooc_new(ooc_stringClass(), "b");
    void* s1b = ooc_new(ooc_stringClass(), "b");
    void* s2b = ooc_new(ooc_stringClass(), "a");

    /* Add in different order */
    ooc_collectionAdd(set1, s1a);
    ooc_collectionAdd(set1, s2a);
    ooc_collectionAdd(set2, s1b);
    ooc_collectionAdd(set2, s2b);

    TEST_ASSERT_TRUE(ooc_equals(set1, set2));

    ooc_destroy(set1);
    ooc_destroy(set2);
}

void test_hash_set_hash_code(void) {
    void* set1 = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* set2 = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* s1a = ooc_new(ooc_stringClass(), "a");
    void* s2a = ooc_new(ooc_stringClass(), "b");
    void* s1b = ooc_new(ooc_stringClass(), "a");
    void* s2b = ooc_new(ooc_stringClass(), "b");

    ooc_collectionAdd(set1, s1a);
    ooc_collectionAdd(set1, s2a);
    ooc_collectionAdd(set2, s1b);
    ooc_collectionAdd(set2, s2b);

    TEST_ASSERT_EQUAL(ooc_hashCode(set1), ooc_hashCode(set2));

    ooc_destroy(set1);
    ooc_destroy(set2);
}

void test_hash_set_to_string(void) {
    void* set = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(set, s1);

    char* str = ooc_toString(set);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_TRUE(str[0] == '{');
    TEST_ASSERT_TRUE(str[strlen(str) - 1] == '}');
    free(str);

    ooc_destroy(set);
}

void test_hash_set_clone(void) {
    void* set = ooc_new(ooc_hashSetClass(), (size_t)16);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(set, s1);
    ooc_collectionAdd(set, s2);

    void* clone = ooc_clone(set);
    TEST_ASSERT_NOT_NULL(clone);
    TEST_ASSERT_TRUE(ooc_equals(set, clone));
    TEST_ASSERT_EQUAL(ooc_collectionSize(set), ooc_collectionSize(clone));

    /* Verify independence */
    void* s3 = ooc_new(ooc_stringClass(), "c");
    ooc_collectionAdd(clone, s3);
    TEST_ASSERT_EQUAL(2, ooc_collectionSize(set));
    TEST_ASSERT_EQUAL(3, ooc_collectionSize(clone));

    ooc_destroy(set);
    ooc_destroy(clone);
}

void test_hash_set_empty(void) {
    void* set = ooc_new(ooc_hashSetClass(), (size_t)16);
    TEST_ASSERT_TRUE(ooc_collectionIsEmpty(set));
    TEST_ASSERT_EQUAL(0, ooc_collectionSize(set));

    void* it = ooc_iterableGetIterator(set);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_FALSE(ooc_iteratorHasNext(it));
    ooc_destroy(it);

    ooc_destroy(set);
}
