#include "unity.h"
#include "oocArrayList.h"
#include "oocString.h"
#include "oocObject.h"
#include "oocError.h"
#include "oocCollection.h"
#include "oocList.h"
#include "oocIterable.h"
#include "oocIterator.h"
#include "oocListIterator.h"
#include <stdlib.h>
#include <string.h>

void test_array_list_create_destroy(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_TRUE(ooc_isInstanceOf(list, ooc_arrayListClass()));
    ooc_destroy(list);
}

void test_array_list_add_and_size(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    TEST_ASSERT_EQUAL(0, ooc_collectionSize(list));

    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_collectionAdd(list, s1));
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_collectionAdd(list, s2));
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_collectionAdd(list, s3));
    TEST_ASSERT_EQUAL(3, ooc_collectionSize(list));

    ooc_destroy(list);
}

void test_array_list_get_at(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    TEST_ASSERT_EQUAL_PTR(s1, ooc_listGetAt(list, 0));
    TEST_ASSERT_EQUAL_PTR(s2, ooc_listGetAt(list, 1));

    ooc_destroy(list);
}

void test_array_list_set_at(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_listSetAt(list, 0, s2));
    TEST_ASSERT_EQUAL_PTR(s2, ooc_listGetAt(list, 0));

    ooc_destroy(list);
}

void test_array_list_insert_at(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s3);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_listInsertAt(list, 1, s2));
    TEST_ASSERT_EQUAL(3, ooc_collectionSize(list));
    TEST_ASSERT_EQUAL_PTR(s1, ooc_listGetAt(list, 0));
    TEST_ASSERT_EQUAL_PTR(s2, ooc_listGetAt(list, 1));
    TEST_ASSERT_EQUAL_PTR(s3, ooc_listGetAt(list, 2));

    ooc_destroy(list);
}

void test_array_list_remove_at(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_listRemoveAt(list, 0));
    TEST_ASSERT_EQUAL(1, ooc_collectionSize(list));
    TEST_ASSERT_EQUAL_PTR(s2, ooc_listGetAt(list, 0));

    ooc_destroy(list);
}

void test_array_list_contains(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    TEST_ASSERT_TRUE(ooc_collectionContains(list, s1));
    TEST_ASSERT_TRUE(ooc_collectionContains(list, s2));
    TEST_ASSERT_FALSE(ooc_collectionContains(list, s3));

    ooc_destroy(s3);
    ooc_destroy(list);
}

void test_array_list_contains_all(void) {
    void* list1 = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* list2 = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");
    void* t1 = ooc_new(ooc_stringClass(), "a");
    void* t2 = ooc_new(ooc_stringClass(), "b");

    ooc_collectionAdd(list1, s1);
    ooc_collectionAdd(list1, s2);
    ooc_collectionAdd(list1, s3);
    ooc_collectionAdd(list2, t1);
    ooc_collectionAdd(list2, t2);


    TEST_ASSERT_TRUE(ooc_collectionContainsAll(list1, list2));
    TEST_ASSERT_FALSE(ooc_collectionContainsAll(list2, list1));

    ooc_destroy(list1);
    ooc_destroy(list2);
}

void test_array_list_index_of(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* dup = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);
    ooc_collectionAdd(list, dup);

    TEST_ASSERT_EQUAL(0, ooc_listIndexOf(list, s1));

    void* notFound = ooc_new(ooc_stringClass(), "z");
    TEST_ASSERT_EQUAL(-1, ooc_listIndexOf(list, notFound));

    ooc_destroy(notFound);
    ooc_destroy(list);
}

void test_array_list_last_index_of(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* dup = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);
    ooc_collectionAdd(list, dup);

    TEST_ASSERT_EQUAL(2, ooc_listLastIndexOf(list, s1));

    ooc_destroy(list);
}

void test_array_list_clear(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_collectionClear(list));
    TEST_ASSERT_EQUAL(0, ooc_collectionSize(list));
    TEST_ASSERT_TRUE(ooc_collectionIsEmpty(list));

    ooc_destroy(list);
}

void test_array_list_iterator(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);
    ooc_collectionAdd(list, s3);

    void* it = ooc_iterableGetIterator(list);
    TEST_ASSERT_NOT_NULL(it);

    TEST_ASSERT_TRUE(ooc_iteratorHasNext(it));
    TEST_ASSERT_EQUAL_PTR(s1, ooc_iteratorNext(it));
    TEST_ASSERT_TRUE(ooc_iteratorHasNext(it));
    TEST_ASSERT_EQUAL_PTR(s2, ooc_iteratorNext(it));
    TEST_ASSERT_TRUE(ooc_iteratorHasNext(it));
    TEST_ASSERT_EQUAL_PTR(s3, ooc_iteratorNext(it));
    TEST_ASSERT_FALSE(ooc_iteratorHasNext(it));

    ooc_destroy(it);
    ooc_destroy(list);
}

void test_array_list_iterator_remove(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);
    ooc_collectionAdd(list, s3);

    void* it = ooc_iterableGetIterator(list);
    ooc_iteratorNext(it);
    ooc_iteratorNext(it);
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_iteratorRemove(it));

    ooc_destroy(it);
    TEST_ASSERT_EQUAL(2, ooc_collectionSize(list));
    TEST_ASSERT_EQUAL_PTR(s1, ooc_listGetAt(list, 0));
    TEST_ASSERT_EQUAL_PTR(s3, ooc_listGetAt(list, 1));

    ooc_destroy(list);
}

void test_array_list_list_iterator(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    void* it = ooc_listGetListIterator(list);
    TEST_ASSERT_NOT_NULL(it);

    TEST_ASSERT_FALSE(ooc_listIteratorHasPrevious(it));
    TEST_ASSERT_EQUAL(0, ooc_listIteratorNextIndex(it));
    TEST_ASSERT_EQUAL(-1, ooc_listIteratorPreviousIndex(it));

    TEST_ASSERT_TRUE(ooc_listIteratorHasNext(it));
    TEST_ASSERT_EQUAL_PTR(s1, ooc_listIteratorNext(it));
    TEST_ASSERT_EQUAL(1, ooc_listIteratorNextIndex(it));
    TEST_ASSERT_EQUAL(0, ooc_listIteratorPreviousIndex(it));

    TEST_ASSERT_TRUE(ooc_listIteratorHasPrevious(it));
    TEST_ASSERT_EQUAL_PTR(s1, ooc_listIteratorPrevious(it));

    ooc_destroy(it);
    ooc_destroy(list);
}

void test_array_list_list_iterator_at(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);
    ooc_collectionAdd(list, s3);

    void* it = ooc_listGetListIteratorAt(list, 1);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL(1, ooc_listIteratorNextIndex(it));
    TEST_ASSERT_EQUAL_PTR(s2, ooc_listIteratorNext(it));

    ooc_destroy(it);
    ooc_destroy(list);
}

void test_array_list_equals(void) {
    void* list1 = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* list2 = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1a = ooc_new(ooc_stringClass(), "a");
    void* s2a = ooc_new(ooc_stringClass(), "b");
    void* s1b = ooc_new(ooc_stringClass(), "a");
    void* s2b = ooc_new(ooc_stringClass(), "b");

    ooc_collectionAdd(list1, s1a);
    ooc_collectionAdd(list1, s2a);
    ooc_collectionAdd(list2, s1b);
    ooc_collectionAdd(list2, s2b);

    TEST_ASSERT_TRUE(ooc_equals(list1, list2));

    void* s3 = ooc_new(ooc_stringClass(), "c");
    ooc_collectionAdd(list2, s3);
    TEST_ASSERT_FALSE(ooc_equals(list1, list2));

    ooc_destroy(list1);
    ooc_destroy(list2);
}

void test_array_list_hash_code(void) {
    void* list1 = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* list2 = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1a = ooc_new(ooc_stringClass(), "a");
    void* s2a = ooc_new(ooc_stringClass(), "b");
    void* s1b = ooc_new(ooc_stringClass(), "a");
    void* s2b = ooc_new(ooc_stringClass(), "b");

    ooc_collectionAdd(list1, s1a);
    ooc_collectionAdd(list1, s2a);
    ooc_collectionAdd(list2, s1b);
    ooc_collectionAdd(list2, s2b);

    TEST_ASSERT_EQUAL(ooc_hashCode(list1), ooc_hashCode(list2));

    ooc_destroy(list1);
    ooc_destroy(list2);
}

void test_array_list_to_string(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    char* str = ooc_toString(list);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_TRUE(str[0] == '[');
    TEST_ASSERT_TRUE(str[strlen(str) - 1] == ']');
    free(str);

    ooc_destroy(list);
}

void test_array_list_clone(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    void* clone = ooc_clone(list);
    TEST_ASSERT_NOT_NULL(clone);
    TEST_ASSERT_TRUE(ooc_equals(list, clone));
    TEST_ASSERT_TRUE(clone != list);
    TEST_ASSERT_EQUAL(ooc_collectionSize(list), ooc_collectionSize(clone));

    /* Verify independence: modify clone, original unchanged */
    void* s3 = ooc_new(ooc_stringClass(), "c");
    ooc_collectionAdd(clone, s3);
    TEST_ASSERT_EQUAL(2, ooc_collectionSize(list));
    TEST_ASSERT_EQUAL(3, ooc_collectionSize(clone));

    ooc_destroy(list);
    ooc_destroy(clone);
}

void test_array_list_capacity(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)4);
    TEST_ASSERT_TRUE(ooc_arrayListCapacity(list) >= 4);

    ooc_arrayListEnsureCapacity(list, 100);
    TEST_ASSERT_TRUE(ooc_arrayListCapacity(list) >= 100);

    void* s1 = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(list, s1);
    ooc_arrayListTrimToSize(list);
    TEST_ASSERT_EQUAL(1, ooc_arrayListCapacity(list));

    ooc_destroy(list);
}

void test_array_list_empty_list(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    TEST_ASSERT_TRUE(ooc_collectionIsEmpty(list));
    TEST_ASSERT_EQUAL(0, ooc_collectionSize(list));

    void* it = ooc_iterableGetIterator(list);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_FALSE(ooc_iteratorHasNext(it));
    ooc_destroy(it);

    ooc_destroy(list);
}

void test_array_list_interface_dispatch(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_listAdd(list, s1));
    TEST_ASSERT_EQUAL(1, ooc_listSize(list));
    TEST_ASSERT_FALSE(ooc_listIsEmpty(list));
    TEST_ASSERT_TRUE(ooc_listContains(list, s1));
    TEST_ASSERT_EQUAL_PTR(s1, ooc_listGetAt(list, 0));

    ooc_destroy(list);
}

void test_array_list_remove_by_value(void) {
    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);
    ooc_collectionAdd(list, s3);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_collectionRemove(list, s2));
    TEST_ASSERT_EQUAL(2, ooc_collectionSize(list));
    void* removedProbe = ooc_new(ooc_stringClass(), "b");
    TEST_ASSERT_FALSE(ooc_collectionContains(list, removedProbe));
    ooc_destroy(removedProbe);

    ooc_destroy(list);
}
