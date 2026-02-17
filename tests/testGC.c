#include "unity.h"
#include "testGC.h"
#include "oocObject.h"
#include "oocGC.h"
#include "oocString.h"
#include "oocHashMap.h"
#include "oocArrayList.h"
#include "oocLinkedList.h"
#include "oocMap.h"
#include "oocCollection.h"

void test_gc_init_shutdown(void) {
    ooc_gcInit();
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());
    ooc_gcShutdown();
}

void test_gc_object_tracking(void) {
    ooc_gcInit();

    void* obj1 = ooc_new(ooc_objectClass());
    TEST_ASSERT_NOT_NULL(obj1);
    TEST_ASSERT_EQUAL(1, ooc_gcObjectCount());

    void* obj2 = ooc_new(ooc_objectClass());
    TEST_ASSERT_NOT_NULL(obj2);
    TEST_ASSERT_EQUAL(2, ooc_gcObjectCount());

    ooc_destroy(obj1);
    TEST_ASSERT_EQUAL(1, ooc_gcObjectCount());

    ooc_destroy(obj2);
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());

    ooc_gcShutdown();
}

void test_gc_sweep_unreachable(void) {
    ooc_gcInit();

    ooc_new(ooc_objectClass());
    ooc_new(ooc_objectClass());
    ooc_new(ooc_objectClass());
    TEST_ASSERT_EQUAL(3, ooc_gcObjectCount());

    /* No roots registered, so all objects are unreachable */
    ooc_gcRun();
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());

    ooc_gcShutdown();
}

void test_gc_keep_reachable(void) {
    ooc_gcInit();

    void* root_obj = ooc_new(ooc_objectClass());
    void* other = ooc_new(ooc_objectClass());
    TEST_ASSERT_EQUAL(2, ooc_gcObjectCount());

    ooc_gcAddRoot((void**)&root_obj);

    ooc_gcRun();

    /* root_obj is reachable, other is not */
    TEST_ASSERT_EQUAL(1, ooc_gcObjectCount());
    TEST_ASSERT_NOT_NULL(root_obj);

    (void)other; /* suppress unused warning */

    ooc_gcRemoveRoot((void**)&root_obj);
    ooc_gcRun();
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());

    ooc_gcShutdown();
}

void test_gc_multiple_roots(void) {
    ooc_gcInit();

    void* a = ooc_new(ooc_objectClass());
    void* b = ooc_new(ooc_objectClass());
    void* c = ooc_new(ooc_objectClass());
    TEST_ASSERT_EQUAL(3, ooc_gcObjectCount());

    ooc_gcAddRoot((void**)&a);
    ooc_gcAddRoot((void**)&c);

    ooc_gcRun();

    /* a and c survive, b is swept */
    TEST_ASSERT_EQUAL(2, ooc_gcObjectCount());

    ooc_gcRemoveRoot((void**)&a);
    ooc_gcRemoveRoot((void**)&c);

    (void)b;

    ooc_gcShutdown();
}

void test_gc_hashmap_with_strings(void) {
    ooc_gcInit();

    void* map = ooc_new(ooc_hashMapClass(), (size_t)0);
    TEST_ASSERT_NOT_NULL(map);

    void* key = ooc_new(ooc_stringClass(), "hello");
    void* val = ooc_new(ooc_stringClass(), "world");
    TEST_ASSERT_NOT_NULL(key);
    TEST_ASSERT_NOT_NULL(val);

    ooc_hashMapPut(map, key, val);

    /* Root the map — all reachable objects should survive GC */
    ooc_gcAddRoot((void**)&map);
    size_t before = ooc_gcObjectCount();
    TEST_ASSERT_TRUE(before > 0);

    ooc_gcRun();

    /* Map and all its children survived */
    TEST_ASSERT_EQUAL(before, ooc_gcObjectCount());

    /* Verify the map still works */
    void* got = ooc_mapGet(map, key);
    TEST_ASSERT_TRUE(ooc_equals(got, val));

    /* Remove root and sweep — everything should be collected */
    ooc_gcRemoveRoot((void**)&map);
    ooc_gcRun();
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());

    ooc_gcShutdown();
}

void test_gc_arraylist_with_strings(void) {
    ooc_gcInit();

    void* list = ooc_new(ooc_arrayListClass(), (size_t)0);
    TEST_ASSERT_NOT_NULL(list);

    void* s1 = ooc_new(ooc_stringClass(), "alpha");
    void* s2 = ooc_new(ooc_stringClass(), "beta");
    void* s3 = ooc_new(ooc_stringClass(), "gamma");
    ooc_collectionAdd(list, s1);
    ooc_collectionAdd(list, s2);
    ooc_collectionAdd(list, s3);

    ooc_gcAddRoot((void**)&list);
    size_t before = ooc_gcObjectCount();

    ooc_gcRun();

    /* All objects reachable from the list survive */
    TEST_ASSERT_EQUAL(before, ooc_gcObjectCount());
    TEST_ASSERT_EQUAL(3, ooc_collectionSize(list));
    TEST_ASSERT_TRUE(ooc_collectionContains(list, s2));

    ooc_gcRemoveRoot((void**)&list);
    ooc_gcRun();
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());

    ooc_gcShutdown();
}

void test_gc_nested_collections(void) {
    ooc_gcInit();

    /* HashMap containing String keys and LinkedList values with String elements */
    void* map = ooc_new(ooc_hashMapClass(), (size_t)0);
    void* key = ooc_new(ooc_stringClass(), "fruits");
    void* innerList = ooc_new(ooc_linkedListClass());
    void* apple = ooc_new(ooc_stringClass(), "apple");
    void* banana = ooc_new(ooc_stringClass(), "banana");

    ooc_collectionAdd(innerList, apple);
    ooc_collectionAdd(innerList, banana);
    ooc_hashMapPut(map, key, innerList);

    ooc_gcAddRoot((void**)&map);
    size_t before = ooc_gcObjectCount();

    ooc_gcRun();

    /* Everything reachable through map -> entry -> linkedList -> strings survives */
    TEST_ASSERT_EQUAL(before, ooc_gcObjectCount());
    TEST_ASSERT_EQUAL(2, ooc_collectionSize(ooc_mapGet(map, key)));

    ooc_gcRemoveRoot((void**)&map);
    ooc_gcRun();
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());

    ooc_gcShutdown();
}
