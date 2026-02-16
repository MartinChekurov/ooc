#include "unity.h"
#include "oocError.h"
#include "oocInterface.h"
#include "oocIterable.h"
#include "oocCollection.h"
#include "oocList.h"
#include "oocSet.h"
#include "oocQueue.h"
#include "oocDeque.h"
#include "oocIterator.h"
#include "oocListIterator.h"
#include "oocMap.h"
#include "oocAbstractCollection.h"
#include "oocAbstractList.h"
#include "oocAbstractSequentialList.h"
#include "oocAbstractSet.h"
#include "oocAbstractIterator.h"
#include "oocAbstractListIterator.h"
#include "oocAbstractMap.h"
#include "oocString.h"
#include "oocObject.h"

void test_interface_and_abstract_class_singletons(void) {
    TEST_ASSERT_NOT_NULL(ooc_interfaceClass());
    TEST_ASSERT_EQUAL_PTR(ooc_interfaceClass(), ooc_interfaceClass());

    TEST_ASSERT_NOT_NULL(ooc_iterableClass());
    TEST_ASSERT_EQUAL_PTR(ooc_iterableClass(), ooc_iterableClass());
    TEST_ASSERT_NOT_NULL(ooc_collectionClass());
    TEST_ASSERT_NOT_NULL(ooc_listClass());
    TEST_ASSERT_NOT_NULL(ooc_setClass());
    TEST_ASSERT_NOT_NULL(ooc_queueClass());
    TEST_ASSERT_NOT_NULL(ooc_dequeClass());
    TEST_ASSERT_NOT_NULL(ooc_iteratorClass());
    TEST_ASSERT_NOT_NULL(ooc_listIteratorClass());
    TEST_ASSERT_NOT_NULL(ooc_mapClass());

    TEST_ASSERT_NOT_NULL(ooc_abstractCollectionClass());
    TEST_ASSERT_NOT_NULL(ooc_abstractListClass());
    TEST_ASSERT_NOT_NULL(ooc_abstractSequentialListClass());
    TEST_ASSERT_NOT_NULL(ooc_abstractSetClass());
    TEST_ASSERT_NOT_NULL(ooc_abstractIteratorClass());
    TEST_ASSERT_NOT_NULL(ooc_abstractListIteratorClass());
    TEST_ASSERT_NOT_NULL(ooc_abstractMapClass());
}

void test_collection_interface_null_safety(void) {
    TEST_ASSERT_NULL(ooc_collectionGetIterator(NULL));
    TEST_ASSERT_EQUAL_UINT64(0, ooc_collectionSize(NULL));
    TEST_ASSERT_TRUE(ooc_collectionIsEmpty(NULL));
    TEST_ASSERT_FALSE(ooc_collectionContains(NULL, NULL));
    TEST_ASSERT_FALSE(ooc_collectionContainsAll(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NULL_POINTER, ooc_collectionAdd(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_collectionRemove(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NULL_POINTER, ooc_collectionClear(NULL));
}

void test_list_interface_null_safety(void) {
    TEST_ASSERT_NULL(ooc_listGetIterator(NULL));
    TEST_ASSERT_EQUAL_UINT64(0, ooc_listSize(NULL));
    TEST_ASSERT_TRUE(ooc_listIsEmpty(NULL));
    TEST_ASSERT_FALSE(ooc_listContains(NULL, NULL));
    TEST_ASSERT_FALSE(ooc_listContainsAll(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NULL_POINTER, ooc_listAdd(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_listRemove(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NULL_POINTER, ooc_listClear(NULL));
    TEST_ASSERT_NULL(ooc_listGetAt(NULL, 0));
    TEST_ASSERT_EQUAL(OOC_ERROR_NULL_POINTER, ooc_listSetAt(NULL, 0, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NULL_POINTER, ooc_listInsertAt(NULL, 0, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_listRemoveAt(NULL, 0));
    TEST_ASSERT_EQUAL(-1, ooc_listIndexOf(NULL, NULL));
    TEST_ASSERT_EQUAL(-1, ooc_listLastIndexOf(NULL, NULL));
    TEST_ASSERT_NULL(ooc_listGetListIterator(NULL));
    TEST_ASSERT_NULL(ooc_listGetListIteratorAt(NULL, 0));
}

void test_set_interface_null_safety(void) {
    TEST_ASSERT_NULL(ooc_setGetIterator(NULL));
    TEST_ASSERT_EQUAL_UINT64(0, ooc_setSize(NULL));
    TEST_ASSERT_TRUE(ooc_setIsEmpty(NULL));
    TEST_ASSERT_FALSE(ooc_setContains(NULL, NULL));
    TEST_ASSERT_FALSE(ooc_setContainsAll(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NULL_POINTER, ooc_setAdd(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_setRemove(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NULL_POINTER, ooc_setClear(NULL));
}

void test_queue_and_deque_interface_null_safety(void) {
    TEST_ASSERT_EQUAL(OOC_ERROR_NULL_POINTER, ooc_queueOffer(NULL, NULL));
    TEST_ASSERT_NULL(ooc_queuePoll(NULL));
    TEST_ASSERT_NULL(ooc_queuePeek(NULL));

    TEST_ASSERT_EQUAL(OOC_ERROR_NULL_POINTER, ooc_dequeAddFirst(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NULL_POINTER, ooc_dequeAddLast(NULL, NULL));
    TEST_ASSERT_NULL(ooc_dequeRemoveFirst(NULL));
    TEST_ASSERT_NULL(ooc_dequeRemoveLast(NULL));
    TEST_ASSERT_NULL(ooc_dequeGetFirst(NULL));
    TEST_ASSERT_NULL(ooc_dequeGetLast(NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_dequePush(NULL, NULL));
    TEST_ASSERT_NULL(ooc_dequeGetPop(NULL));
}

void test_iterator_interface_null_safety(void) {
    TEST_ASSERT_FALSE(ooc_iteratorHasNext(NULL));
    TEST_ASSERT_NULL(ooc_iteratorNext(NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_iteratorRemove(NULL));

    TEST_ASSERT_FALSE(ooc_listIteratorHasPrevious(NULL));
    TEST_ASSERT_NULL(ooc_listIteratorPrevious(NULL));
    TEST_ASSERT_EQUAL(-1, ooc_listIteratorNextIndex(NULL));
    TEST_ASSERT_EQUAL(-1, ooc_listIteratorPreviousIndex(NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_listIteratorSet(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_listIteratorAdd(NULL, NULL));
}

void test_map_interface_null_safety(void) {
    TEST_ASSERT_NULL(ooc_mapGetIterator(NULL));
    TEST_ASSERT_EQUAL_UINT64(0, ooc_mapSize(NULL));
    TEST_ASSERT_TRUE(ooc_mapIsEmpty(NULL));
    TEST_ASSERT_FALSE(ooc_mapContainsKey(NULL, NULL));
    TEST_ASSERT_FALSE(ooc_mapContainsValue(NULL, NULL));
    TEST_ASSERT_NULL(ooc_mapGet(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_mapPut(NULL, NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_mapRemove(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_mapClear(NULL));
    TEST_ASSERT_NULL(ooc_mapKeySet(NULL));
    TEST_ASSERT_NULL(ooc_mapValues(NULL));
}

void test_interface_non_implementer_behavior(void) {
    void* str = ooc_new(ooc_stringClass(), "plain object");
    TEST_ASSERT_NOT_NULL(str);

    TEST_ASSERT_NULL(ooc_iterableGetIterator(str));
    TEST_ASSERT_EQUAL_UINT64(0, ooc_collectionSize(str));
    TEST_ASSERT_TRUE(ooc_collectionIsEmpty(str));
    TEST_ASSERT_FALSE(ooc_collectionContains(str, NULL));
    TEST_ASSERT_FALSE(ooc_collectionContainsAll(str, str));
    TEST_ASSERT_EQUAL(OOC_ERROR_NO_CLASS, ooc_collectionAdd(str, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NO_CLASS, ooc_collectionRemove(str, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NO_CLASS, ooc_collectionClear(str));

    TEST_ASSERT_NULL(ooc_listGetAt(str, 0));
    TEST_ASSERT_EQUAL(OOC_ERROR_NO_CLASS, ooc_listSetAt(str, 0, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NO_CLASS, ooc_listInsertAt(str, 0, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NO_CLASS, ooc_listRemoveAt(str, 0));
    TEST_ASSERT_EQUAL(-1, ooc_listIndexOf(str, NULL));
    TEST_ASSERT_EQUAL(-1, ooc_listLastIndexOf(str, NULL));
    TEST_ASSERT_NULL(ooc_listGetListIterator(str));
    TEST_ASSERT_NULL(ooc_listGetListIteratorAt(str, 0));

    TEST_ASSERT_EQUAL(OOC_ERROR_NOT_IMPLEMENTED, ooc_mapPut(str, NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NOT_IMPLEMENTED, ooc_mapRemove(str, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_NOT_IMPLEMENTED, ooc_mapClear(str));
    TEST_ASSERT_NULL(ooc_mapGetIterator(str));

    ooc_destroy(str);
}
