#include "unity.h"
#include "oocLinkedList.h"
#include "oocArrayList.h"
#include "oocString.h"
#include "oocObject.h"
#include "oocError.h"
#include "oocCollection.h"
#include "oocList.h"
#include "oocQueue.h"
#include "oocDeque.h"
#include "oocIterable.h"
#include "oocIterator.h"
#include "oocListIterator.h"
#include <stdlib.h>

void test_linked_list_create_destroy(void) {
    void* list = ooc_new(ooc_linkedListClass());
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_TRUE(ooc_isInstanceOf(list, ooc_linkedListClass()));
    ooc_destroy(list);
}

void test_linked_list_add_and_size(void) {
    void* list = ooc_new(ooc_linkedListClass());
    TEST_ASSERT_EQUAL(0, ooc_collectionSize(list));

    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);
    TEST_ASSERT_EQUAL(2, ooc_collectionSize(list));

    ooc_destroy(list);
}

void test_linked_list_get_at(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    TEST_ASSERT_EQUAL_PTR(s1, ooc_listGetAt(list, 0));
    TEST_ASSERT_EQUAL_PTR(s2, ooc_listGetAt(list, 1));

    ooc_destroy(list);
}

void test_linked_list_set_at(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_listSetAt(list, 0, s2));
    TEST_ASSERT_EQUAL_PTR(s2, ooc_listGetAt(list, 0));

    ooc_destroy(list);
}

void test_linked_list_insert_at(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s3);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_listInsertAt(list, 1, s2));
    TEST_ASSERT_EQUAL(3, ooc_collectionSize(list));
    TEST_ASSERT_EQUAL_PTR(s2, ooc_listGetAt(list, 1));

    ooc_destroy(list);
}

void test_linked_list_remove_at(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_listRemoveAt(list, 0));
    TEST_ASSERT_EQUAL(1, ooc_collectionSize(list));
    TEST_ASSERT_EQUAL_PTR(s2, ooc_listGetAt(list, 0));

    ooc_destroy(list);
}

void test_linked_list_contains(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);

    TEST_ASSERT_TRUE(ooc_collectionContains(list, s1));
    TEST_ASSERT_FALSE(ooc_collectionContains(list, s2));

    ooc_destroy(s2);
    ooc_destroy(list);
}

void test_linked_list_index_of(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    TEST_ASSERT_EQUAL(0, ooc_listIndexOf(list, s1));
    TEST_ASSERT_EQUAL(1, ooc_listIndexOf(list, s2));

    ooc_destroy(list);
}

void test_linked_list_last_index_of(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* dup = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);
    ooc_collectionAdd(list, dup);

    TEST_ASSERT_EQUAL(2, ooc_listLastIndexOf(list, s1));

    ooc_destroy(list);
}

void test_linked_list_clear(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(list, s1);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_collectionClear(list));
    TEST_ASSERT_EQUAL(0, ooc_collectionSize(list));

    ooc_destroy(list);
}

void test_linked_list_iterator(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    void* it = ooc_iterableGetIterator(list);
    TEST_ASSERT_TRUE(ooc_iteratorHasNext(it));
    TEST_ASSERT_EQUAL_PTR(s1, ooc_iteratorNext(it));
    TEST_ASSERT_EQUAL_PTR(s2, ooc_iteratorNext(it));
    TEST_ASSERT_FALSE(ooc_iteratorHasNext(it));

    ooc_destroy(it);
    ooc_destroy(list);
}

void test_linked_list_iterator_remove(void) {
    void* list = ooc_new(ooc_linkedListClass());
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
    ooc_destroy(list);
}

void test_linked_list_list_iterator(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    void* it = ooc_listGetListIterator(list);
    TEST_ASSERT_NOT_NULL(it);

    TEST_ASSERT_FALSE(ooc_listIteratorHasPrevious(it));
    TEST_ASSERT_EQUAL_PTR(s1, ooc_listIteratorNext(it));
    TEST_ASSERT_TRUE(ooc_listIteratorHasPrevious(it));
    TEST_ASSERT_EQUAL_PTR(s1, ooc_listIteratorPrevious(it));

    ooc_destroy(it);
    ooc_destroy(list);
}

void test_linked_list_equals_with_arraylist(void) {
    void* linked = ooc_new(ooc_linkedListClass());
    void* array = ooc_new(ooc_arrayListClass(), (size_t)0);

    void* s1a = ooc_new(ooc_stringClass(), "a");
    void* s2a = ooc_new(ooc_stringClass(), "b");
    void* s1b = ooc_new(ooc_stringClass(), "a");
    void* s2b = ooc_new(ooc_stringClass(), "b");

    ooc_collectionAdd(linked, s1a);
    ooc_collectionAdd(linked, s2a);
    ooc_collectionAdd(array, s1b);
    ooc_collectionAdd(array, s2b);

    TEST_ASSERT_TRUE(ooc_equals(linked, array));
    TEST_ASSERT_TRUE(ooc_equals(array, linked));

    ooc_destroy(linked);
    ooc_destroy(array);
}

void test_linked_list_queue_operations(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_queueOffer(list, s1));
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_queueOffer(list, s2));

    TEST_ASSERT_EQUAL_PTR(s1, ooc_queuePeek(list));
    TEST_ASSERT_EQUAL(2, ooc_collectionSize(list));

    void* polled = ooc_queuePoll(list);
    TEST_ASSERT_EQUAL_PTR(s1, polled);
    TEST_ASSERT_EQUAL(1, ooc_collectionSize(list));

    ooc_destroy(polled);
    ooc_destroy(ooc_queuePoll(list));
    ooc_destroy(list);
}

void test_linked_list_deque_operations(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");

    ooc_dequeAddFirst(list, s2);
    ooc_dequeAddFirst(list, s1);
    ooc_dequeAddLast(list, s3);

    TEST_ASSERT_EQUAL_PTR(s1, ooc_dequeGetFirst(list));
    TEST_ASSERT_EQUAL_PTR(s3, ooc_dequeGetLast(list));
    TEST_ASSERT_EQUAL(3, ooc_collectionSize(list));

    void* first = ooc_dequeRemoveFirst(list);
    TEST_ASSERT_EQUAL_PTR(s1, first);

    void* last = ooc_dequeRemoveLast(list);
    TEST_ASSERT_EQUAL_PTR(s3, last);

    TEST_ASSERT_EQUAL(1, ooc_collectionSize(list));

    ooc_destroy(first);
    ooc_destroy(last);
    ooc_destroy(ooc_dequeRemoveFirst(list));
    ooc_destroy(list);
}

void test_linked_list_stack_operations(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");

    ooc_dequePush(list, s1);
    ooc_dequePush(list, s2);

    void* popped = ooc_dequeGetPop(list);
    TEST_ASSERT_EQUAL_PTR(s2, popped);
    ooc_destroy(popped);

    popped = ooc_dequeGetPop(list);
    TEST_ASSERT_EQUAL_PTR(s1, popped);
    ooc_destroy(popped);

    TEST_ASSERT_TRUE(ooc_collectionIsEmpty(list));

    ooc_destroy(list);
}

void test_linked_list_empty_deque_edge_cases(void) {
    void* list = ooc_new(ooc_linkedListClass());

    TEST_ASSERT_NULL(ooc_queuePoll(list));
    TEST_ASSERT_NULL(ooc_queuePeek(list));
    TEST_ASSERT_NULL(ooc_dequeGetFirst(list));
    TEST_ASSERT_NULL(ooc_dequeGetLast(list));

    ooc_destroy(list);
}

void test_linked_list_clone(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    void* clone = ooc_clone(list);
    TEST_ASSERT_NOT_NULL(clone);
    TEST_ASSERT_TRUE(ooc_equals(list, clone));
    TEST_ASSERT_EQUAL(ooc_collectionSize(list), ooc_collectionSize(clone));

    ooc_destroy(list);
    ooc_destroy(clone);
}

void test_linked_list_to_string(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(list, s1);

    char* str = ooc_toString(list);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_TRUE(str[0] == '[');
    free(str);

    ooc_destroy(list);
}

void test_linked_list_hash_code(void) {
    void* list1 = ooc_new(ooc_linkedListClass());
    void* list2 = ooc_new(ooc_linkedListClass());
    void* s1a = ooc_new(ooc_stringClass(), "a");
    void* s1b = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(list1, s1a);
    ooc_collectionAdd(list2, s1b);

    TEST_ASSERT_EQUAL(ooc_hashCode(list1), ooc_hashCode(list2));

    ooc_destroy(list1);
    ooc_destroy(list2);
}

void test_linked_list_remove_by_value(void) {
    void* list = ooc_new(ooc_linkedListClass());
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_collectionRemove(list, s1));
    TEST_ASSERT_EQUAL(1, ooc_collectionSize(list));
    void* removedProbe = ooc_new(ooc_stringClass(), "a");
    TEST_ASSERT_FALSE(ooc_collectionContains(list, removedProbe));
    ooc_destroy(removedProbe);

    ooc_destroy(list);
}
