#include "unity.h"
#include "oocObject.h"
#include "oocError.h"
#include <stdlib.h>
#include <string.h>

void test_object_class_creation(void) {
    void* objClass = ooc_objectClass();
    TEST_ASSERT_NOT_NULL(objClass);
}

void test_object_creation_and_destruction(void) {
    void* obj = ooc_new(ooc_objectClass());
    TEST_ASSERT_NOT_NULL(obj);

    OOC_Error result = ooc_destroy(obj);
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, result);
}

void test_object_class_info(void) {
    void* obj = ooc_new(ooc_objectClass());
    TEST_ASSERT_NOT_NULL(obj);

    void* objClass = ooc_classOf(obj);
    TEST_ASSERT_NOT_NULL(objClass);
    TEST_ASSERT_EQUAL_PTR(objClass, ooc_objectClass());

    const char* className = ooc_getClassName(obj);
    TEST_ASSERT_NOT_NULL(className);
    TEST_ASSERT_EQUAL_STRING("Object", className);

    TEST_ASSERT_FALSE(ooc_isAbstract(obj));
    TEST_ASSERT_FALSE(ooc_isFinal(obj));

    size_t size = ooc_sizeOf(obj);
    TEST_ASSERT_GREATER_THAN(0, size);

    ooc_destroy(obj);
}

void test_object_instanceof(void) {
    void* obj = ooc_new(ooc_objectClass());
    TEST_ASSERT_NOT_NULL(obj);

    TEST_ASSERT_TRUE(ooc_isInstanceOf(obj, ooc_objectClass()));

    ooc_destroy(obj);
}

void test_object_equality(void) {
    void* obj1 = ooc_new(ooc_objectClass());
    void* obj2 = ooc_new(ooc_objectClass());
    TEST_ASSERT_NOT_NULL(obj1);
    TEST_ASSERT_NOT_NULL(obj2);

    // Different objects should not be equal
    TEST_ASSERT_FALSE(ooc_equals(obj1, obj2));

    // Object should be equal to itself
    TEST_ASSERT_TRUE(ooc_equals(obj1, obj1));

    ooc_destroy(obj1);
    ooc_destroy(obj2);
}

void test_object_hash_code(void) {
    void* obj = ooc_new(ooc_objectClass());
    TEST_ASSERT_NOT_NULL(obj);

    size_t hash1 = ooc_hashCode(obj);
    size_t hash2 = ooc_hashCode(obj);

    // Hash code should be consistent
    TEST_ASSERT_EQUAL_UINT(hash1, hash2);

    ooc_destroy(obj);
}

void test_object_to_string(void) {
    void* obj = ooc_new(ooc_objectClass());
    TEST_ASSERT_NOT_NULL(obj);

    const char* str = ooc_toString(obj);
    TEST_ASSERT_NOT_NULL(str);

    // Should contain "Object" and the address
    TEST_ASSERT_NOT_NULL(strstr(str, "Object"));
    TEST_ASSERT_NOT_NULL(strstr(str, "@"));

    free((void*)str); // toString returns allocated memory
    ooc_destroy(obj);
}

void test_object_clone(void) {
    void* obj = ooc_new(ooc_objectClass());
    TEST_ASSERT_NOT_NULL(obj);

    void* clone = ooc_clone(obj);
    TEST_ASSERT_NOT_NULL(clone);
    TEST_ASSERT_NOT_EQUAL(obj, clone); // Different objects

    ooc_destroy(obj);
    ooc_destroy(clone);
}
