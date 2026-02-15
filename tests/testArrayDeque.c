#include "unity.h"
#include "oocArrayDeque.h"
#include "oocString.h"
#include "oocObject.h"
#include "oocError.h"
#include "oocCollection.h"
#include "oocQueue.h"
#include "oocDeque.h"
#include "oocIterable.h"
#include "oocIterator.h"
#include <stdlib.h>
#include <string.h>

void test_array_deque_create_destroy(void) {
    void* deque = ooc_new(ooc_arrayDequeClass(), (size_t)0);
    TEST_ASSERT_NOT_NULL(deque);
    TEST_ASSERT_TRUE(ooc_isInstanceOf(deque, ooc_arrayDequeClass()));
    ooc_destroy(deque);
}

void test_array_deque_queue_operations(void) {
    void* deque = ooc_new(ooc_arrayDequeClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_queueOffer(deque, s1));
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_queueOffer(deque, s2));
    TEST_ASSERT_EQUAL(2, ooc_collectionSize(deque));

    TEST_ASSERT_EQUAL_PTR(s1, ooc_queuePeek(deque));
    void* polled = ooc_queuePoll(deque);
    TEST_ASSERT_EQUAL_PTR(s1, polled);
    TEST_ASSERT_EQUAL(1, ooc_collectionSize(deque));

    ooc_destroy(deque);
}

void test_array_deque_deque_operations(void) {
    void* deque = ooc_new(ooc_arrayDequeClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");

    ooc_dequeAddFirst(deque, s2);
    ooc_dequeAddFirst(deque, s1);
    ooc_dequeAddLast(deque, s3);

    TEST_ASSERT_EQUAL_PTR(s1, ooc_dequeGetFirst(deque));
    TEST_ASSERT_EQUAL_PTR(s3, ooc_dequeGetLast(deque));
    TEST_ASSERT_EQUAL(3, ooc_collectionSize(deque));

    void* first = ooc_dequeRemoveFirst(deque);
    TEST_ASSERT_EQUAL_PTR(s1, first);

    void* last = ooc_dequeRemoveLast(deque);
    TEST_ASSERT_EQUAL_PTR(s3, last);

    TEST_ASSERT_EQUAL(1, ooc_collectionSize(deque));

    ooc_destroy(deque);
}

void test_array_deque_stack_operations(void) {
    void* deque = ooc_new(ooc_arrayDequeClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");

    ooc_dequePush(deque, s1);
    ooc_dequePush(deque, s2);

    void* popped = ooc_dequeGetPop(deque);
    TEST_ASSERT_EQUAL_PTR(s2, popped);

    popped = ooc_dequeGetPop(deque);
    TEST_ASSERT_EQUAL_PTR(s1, popped);

    TEST_ASSERT_TRUE(ooc_collectionIsEmpty(deque));

    ooc_destroy(deque);
}

void test_array_deque_contains(void) {
    void* deque = ooc_new(ooc_arrayDequeClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");

    ooc_collectionAdd(deque, s1);
    TEST_ASSERT_TRUE(ooc_collectionContains(deque, s1));
    TEST_ASSERT_FALSE(ooc_collectionContains(deque, s2));

    ooc_destroy(s2);
    ooc_destroy(deque);
}

void test_array_deque_remove_by_value(void) {
    void* deque = ooc_new(ooc_arrayDequeClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");

    ooc_collectionAdd(deque, s1);
    ooc_collectionAdd(deque, s2);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_collectionRemove(deque, s1));
    TEST_ASSERT_EQUAL(1, ooc_collectionSize(deque));
    TEST_ASSERT_FALSE(ooc_collectionContains(deque, s1));

    ooc_destroy(deque);
}

void test_array_deque_size_and_clear(void) {
    void* deque = ooc_new(ooc_arrayDequeClass(), (size_t)0);
    TEST_ASSERT_TRUE(ooc_collectionIsEmpty(deque));

    void* s1 = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(deque, s1);
    TEST_ASSERT_EQUAL(1, ooc_collectionSize(deque));
    TEST_ASSERT_FALSE(ooc_collectionIsEmpty(deque));

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_collectionClear(deque));
    TEST_ASSERT_EQUAL(0, ooc_collectionSize(deque));
    TEST_ASSERT_TRUE(ooc_collectionIsEmpty(deque));

    ooc_destroy(deque);
}

void test_array_deque_iterator(void) {
    void* deque = ooc_new(ooc_arrayDequeClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    void* s3 = ooc_new(ooc_stringClass(), "c");

    ooc_collectionAdd(deque, s1);
    ooc_collectionAdd(deque, s2);
    ooc_collectionAdd(deque, s3);

    void* it = ooc_iterableGetIterator(deque);
    TEST_ASSERT_NOT_NULL(it);

    int count = 0;
    while (ooc_iteratorHasNext(it)) {
        void* elem = ooc_iteratorNext(it);
        TEST_ASSERT_NOT_NULL(elem);
        count++;
    }
    TEST_ASSERT_EQUAL(3, count);

    ooc_destroy(it);
    ooc_destroy(deque);
}

void test_array_deque_empty_edge_cases(void) {
    void* deque = ooc_new(ooc_arrayDequeClass(), (size_t)0);

    TEST_ASSERT_NULL(ooc_queuePoll(deque));
    TEST_ASSERT_NULL(ooc_queuePeek(deque));
    TEST_ASSERT_NULL(ooc_dequeGetFirst(deque));
    TEST_ASSERT_NULL(ooc_dequeGetLast(deque));

    ooc_destroy(deque);
}

void test_array_deque_to_string(void) {
    void* deque = ooc_new(ooc_arrayDequeClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    ooc_collectionAdd(deque, s1);

    char* str = ooc_toString(deque);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_TRUE(str[0] == '[');
    TEST_ASSERT_TRUE(str[strlen(str) - 1] == ']');
    free(str);

    ooc_destroy(deque);
}

void test_array_deque_clone(void) {
    void* deque = ooc_new(ooc_arrayDequeClass(), (size_t)0);
    void* s1 = ooc_new(ooc_stringClass(), "a");
    void* s2 = ooc_new(ooc_stringClass(), "b");
    ooc_collectionAdd(deque, s1);
    ooc_collectionAdd(deque, s2);

    void* clone = ooc_clone(deque);
    TEST_ASSERT_NOT_NULL(clone);
    TEST_ASSERT_EQUAL(ooc_collectionSize(deque), ooc_collectionSize(clone));

    ooc_destroy(deque);
    ooc_destroy(clone);
}

void test_array_deque_add_many(void) {
    void* deque = ooc_new(ooc_arrayDequeClass(), (size_t)4);

    /* Add more elements than initial capacity to test resizing */
    for (int i = 0; i < 20; i++) {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d", i);
        void* s = ooc_new(ooc_stringClass(), buf);
        ooc_collectionAdd(deque, s);
    }

    TEST_ASSERT_EQUAL(20, ooc_collectionSize(deque));

    ooc_destroy(deque);
}
