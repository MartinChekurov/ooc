#include "unity.h"
#include "oocObject.h"
#include "oocError.h"
#include "oocString.h"
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

    const void* objClass = ooc_classOf(obj);
    TEST_ASSERT_EQUAL_PTR(ooc_objectClass(), objClass);

    const char* className = ooc_getClassName(obj);
    TEST_ASSERT_EQUAL_STRING("Object", className);

    size_t size = ooc_sizeOf(obj);
    TEST_ASSERT_TRUE(size > 0);

    TEST_ASSERT_FALSE(ooc_isAbstract(obj));
    TEST_ASSERT_FALSE(ooc_isFinal(obj));

    ooc_destroy(obj);
}

void test_object_instanceof(void) {
    void* obj = ooc_new(ooc_objectClass());
    TEST_ASSERT_TRUE(ooc_isInstanceOf(obj, ooc_objectClass()));

    void* str = ooc_new(ooc_stringClass(), "test");
    TEST_ASSERT_TRUE(ooc_isInstanceOf(str, ooc_objectClass()));
    TEST_ASSERT_TRUE(ooc_isInstanceOf(str, ooc_stringClass()));
    TEST_ASSERT_FALSE(ooc_isInstanceOf(obj, ooc_stringClass()));

    ooc_destroy(obj);
    ooc_destroy(str);
}

void test_object_equals(void) {
    void* obj1 = ooc_new(ooc_objectClass());
    void* obj2 = ooc_new(ooc_objectClass());

    TEST_ASSERT_TRUE(ooc_equals(obj1, obj1));
    TEST_ASSERT_FALSE(ooc_equals(obj1, obj2));

    ooc_destroy(obj1);
    ooc_destroy(obj2);
}

void test_object_hash_code(void) {
    void* obj = ooc_new(ooc_objectClass());
    size_t hash1 = ooc_hashCode(obj);
    size_t hash2 = ooc_hashCode(obj);
    TEST_ASSERT_EQUAL(hash1, hash2);
    ooc_destroy(obj);
}

void test_object_to_string(void) {
    void* obj = ooc_new(ooc_objectClass());
    char* str = ooc_toString(obj);
    TEST_ASSERT_NOT_NULL(str);
    free(str);
    ooc_destroy(obj);
}

void test_object_clone(void) {
    void* obj = ooc_new(ooc_objectClass());
    void* clone = ooc_clone(obj);
    TEST_ASSERT_NOT_NULL(clone);
    TEST_ASSERT_TRUE(ooc_isInstanceOf(clone, ooc_objectClass()));
    TEST_ASSERT_TRUE(clone != obj);
    ooc_destroy(obj);
    ooc_destroy(clone);
}

void test_object_destroy_null(void) {
    OOC_Error result = ooc_destroy(NULL);
    TEST_ASSERT_NOT_EQUAL(OOC_ERROR_NONE, result);
}
