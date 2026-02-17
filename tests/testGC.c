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
    OOC_GC_ROOT(map);
    size_t before = ooc_gcObjectCount();
    TEST_ASSERT_TRUE(before > 0);

    ooc_gcRun();

    /* Map and all its children survived */
    TEST_ASSERT_EQUAL(before, ooc_gcObjectCount());

    /* Verify the map still works */
    void* got = ooc_mapGet(map, key);
    TEST_ASSERT_TRUE(ooc_equals(got, val));

    /* Remove root and sweep — everything should be collected */
    OOC_GC_UNROOT(map);
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


void test_gc_scoped_root_macro(void) {
    ooc_gcInit();

#if OOC_GC_HAS_SCOPED_ROOTS
    {
        void* root = ooc_new(ooc_objectClass());
        void* other = ooc_new(ooc_objectClass());
        TEST_ASSERT_EQUAL(2, ooc_gcObjectCount());

        OOC_GC_SCOPED_ROOT(root);
        ooc_gcRun();

        /* Root survives; unrooted object is collected */
        TEST_ASSERT_EQUAL(1, ooc_gcObjectCount());
        TEST_ASSERT_NOT_NULL(root);

        (void)other;
    }

    /* Root was automatically unregistered at scope exit */
    ooc_gcRun();
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());
#else
    TEST_IGNORE_MESSAGE("OOC_GC_SCOPED_ROOT requires GCC/Clang cleanup attribute");
#endif

    ooc_gcShutdown();
}


void test_gc_new_rooted_macro(void) {
    ooc_gcInit();

    void* map = NULL;
    OOC_NEW_ROOTED(map, ooc_hashMapClass(), (size_t)0);
    TEST_ASSERT_NOT_NULL(map);

    void* key = ooc_new(ooc_stringClass(), "k");
    void* val = ooc_new(ooc_stringClass(), "v");
    ooc_hashMapPut(map, key, val);

    ooc_gcRun();

    /* Map was rooted by OOC_NEW_ROOTED, so contents survive */
    TEST_ASSERT_TRUE(ooc_equals(ooc_mapGet(map, key), val));

    OOC_GC_UNROOT(map);
    ooc_gcRun();
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());

    ooc_gcShutdown();
}


void test_gc_new_scoped_macro(void) {
    ooc_gcInit();

#if OOC_GC_HAS_SCOPED_ROOTS
    {
        void* list = NULL;
        OOC_NEW_SCOPED(list, ooc_arrayListClass(), (size_t)0);
        TEST_ASSERT_NOT_NULL(list);

        void* item = ooc_new(ooc_stringClass(), "item");
        ooc_collectionAdd(list, item);

        ooc_gcRun();
        TEST_ASSERT_EQUAL(1, ooc_collectionSize(list));
    }

    /* list root auto-removed on scope exit */
    ooc_gcRun();
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());
#else
    TEST_IGNORE_MESSAGE("OOC_NEW_SCOPED requires GCC/Clang cleanup attribute");
#endif

    ooc_gcShutdown();
}


static void helper_create_rooted_in_caller_slot(void** out) {
    OOC_GC_NEW_IN(out, ooc_stringClass(), "cross-fn");
}

void test_gc_new_in_across_functions(void) {
    ooc_gcInit();

    void* value = NULL;
    helper_create_rooted_in_caller_slot(&value);
    TEST_ASSERT_NOT_NULL(value);

    ooc_gcRun();
    TEST_ASSERT_NOT_NULL(value);

    OOC_GC_UNROOT(value);
    ooc_gcRun();
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());

    ooc_gcShutdown();
}

static void helper_create_and_move_root_to_caller(void** out) {
    void* local = ooc_new(ooc_stringClass(), "moved-root");
    OOC_GC_ROOT(local);

    *out = local;
    OOC_GC_MOVE_ROOT(&local, out);
}

void test_gc_move_root_to_caller_slot(void) {
    ooc_gcInit();

    void* escaped = NULL;
    helper_create_and_move_root_to_caller(&escaped);
    TEST_ASSERT_NOT_NULL(escaped);

    ooc_gcRun();
    TEST_ASSERT_NOT_NULL(escaped);

    OOC_GC_UNROOT(escaped);
    ooc_gcRun();
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());

    ooc_gcShutdown();
}

void test_gc_duplicate_root_registration_is_ignored(void) {
    ooc_gcInit();

    void* rooted = ooc_new(ooc_objectClass());
    void* other = ooc_new(ooc_objectClass());
    TEST_ASSERT_EQUAL(2, ooc_gcObjectCount());

    ooc_gcAddRoot((void**)&rooted);
    ooc_gcAddRoot((void**)&rooted);
    TEST_ASSERT_EQUAL(1, ooc_gcRootCount());

    ooc_gcRun();
    TEST_ASSERT_EQUAL(1, ooc_gcObjectCount());
    TEST_ASSERT_NOT_NULL(rooted);

    ooc_gcRemoveRoot((void**)&rooted);
    ooc_gcRun();
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());

    (void)other;

    ooc_gcShutdown();
}

void test_gc_root_introspection_helpers(void) {
    ooc_gcInit();

    void* rooted = ooc_new(ooc_objectClass());
    TEST_ASSERT_FALSE(ooc_gcHasRoot((void**)&rooted));
    TEST_ASSERT_EQUAL(0, ooc_gcRootCount());

    ooc_gcAddRoot((void**)&rooted);
    TEST_ASSERT_TRUE(ooc_gcHasRoot((void**)&rooted));
    TEST_ASSERT_EQUAL(1, ooc_gcRootCount());

    ooc_gcRemoveRoot((void**)&rooted);
    TEST_ASSERT_FALSE(ooc_gcHasRoot((void**)&rooted));
    TEST_ASSERT_EQUAL(0, ooc_gcRootCount());

    ooc_gcRun();
    TEST_ASSERT_EQUAL(0, ooc_gcObjectCount());

    ooc_gcShutdown();
}
