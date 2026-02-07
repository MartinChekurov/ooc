// #include "unity.h"
// #include "oocObject.h"
// #include "oocObject.r"
// #include "oocError.h"
// #include "oocModifiers.h"
// #include <stdlib.h>
// #include <string.h>
//
// // Test custom class for inheritance testing
// typedef struct TestClass TestClass;
// typedef struct TestObject TestObject;
//
// struct TestObject {
//     struct OOC_Object object;
//     int value;
//     char* data;
// };
//
// struct TestClass {
//     struct OOC_Class class;
// };
//
// static TestClass TestClassInstance;
// static TestClass* CustomTestClass = NULL;
//
// static OOC_Error testConstructor(void* self, va_list* args) {
//     TestObject* obj = (TestObject*)self;
//     obj->value = va_arg(*args, int);
//     const char* str = va_arg(*args, const char*);
//     if (str) {
//         obj->data = strdup(str);
//         if (!obj->data) return OOC_ERROR_OUT_OF_MEMORY;
//     } else {
//         obj->data = NULL;
//     }
//     return OOC_ERROR_NONE;
// }
//
// static OOC_Error testDestructor(void* self) {
//     TestObject* obj = (TestObject*)self;
//     free(obj->data);
//     obj->data = NULL;
//     return OOC_ERROR_NONE;
// }
//
// static char* testToString(void* self) {
//     TestObject* obj = (TestObject*)self;
//     size_t len = snprintf(NULL, 0, "TestObject@%p(value=%d, data=%s)",
//                          self, obj->value, obj->data ? obj->data : "NULL");
//     char* result = malloc(len + 1);
//     if (result) {
//         snprintf(result, len + 1, "TestObject@%p(value=%d, data=%s)",
//                 self, obj->value, obj->data ? obj->data : "NULL");
//     }
//     return result;
// }
//
// static void* getTestClass() {
//     if (!CustomTestClass) {
//         if (ooc_classNew(&TestClassInstance,
//                         "TestObject",
//                         sizeof(TestObject),
//                         sizeof(TestClass),
//                         ooc_objectClass(),
//                         OOC_MODIFIER_NONE,
//                         OOC_METHOD_CTOR, testConstructor,
//                         OOC_METHOD_DTOR, testDestructor,
//                         OOC_METHOD_TO_STRING, testToString,
//                         0) == OOC_ERROR_NONE) {
//             CustomTestClass = &TestClassInstance;
//         }
//     }
//     return CustomTestClass;
// }
//
// void test_core_null_handling(void) {
//     // Test all functions with NULL arguments
//     TEST_ASSERT_NULL(ooc_new(NULL));
//     TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_destroy(NULL));
//     TEST_ASSERT_NULL(ooc_classOf(NULL));
//     TEST_ASSERT_NULL(ooc_super(NULL));
//     TEST_ASSERT_EQUAL_UINT(0, ooc_sizeOf(NULL));
//     TEST_ASSERT_NULL(ooc_getClassName(NULL));
//     TEST_ASSERT_FALSE(ooc_isInstanceOf(NULL, ooc_objectClass()));
//     TEST_ASSERT_FALSE(ooc_isInstanceOf(ooc_new(ooc_objectClass()), NULL));
//     TEST_ASSERT_NULL(ooc_toString(NULL));
//     TEST_ASSERT_FALSE(ooc_equals(NULL, NULL));
//     TEST_ASSERT_EQUAL_UINT(0, ooc_hashCode(NULL));
//     TEST_ASSERT_NULL(ooc_clone(NULL));
// }
//
// void test_core_error_codes(void) {
//     // Test that error codes are properly defined
//     TEST_ASSERT_EQUAL(0, OOC_ERROR_NONE);
//     TEST_ASSERT_NOT_EQUAL(OOC_ERROR_NONE, OOC_ERROR_OUT_OF_MEMORY);
//     TEST_ASSERT_NOT_EQUAL(OOC_ERROR_NONE, OOC_ERROR_INVALID_ARGUMENT);
//     TEST_ASSERT_NOT_EQUAL(OOC_ERROR_NONE, OOC_ERROR_INVALID_OBJECT);
//     TEST_ASSERT_NOT_EQUAL(OOC_ERROR_NONE, OOC_ERROR_NOT_IMPLEMENTED);
//     TEST_ASSERT_NOT_EQUAL(OOC_ERROR_NONE, OOC_ERROR_NO_CLASS);
//     TEST_ASSERT_NOT_EQUAL(OOC_ERROR_NONE, OOC_ERROR_FINAL_CLASS_INHERITANCE);
//     TEST_ASSERT_NOT_EQUAL(OOC_ERROR_NONE, OOC_ERROR_BUFFER_TOO_SMALL);
//     TEST_ASSERT_NOT_EQUAL(OOC_ERROR_NONE, OOC_ERROR_STANDARD);
//
//     // Ensure error count is last
//     TEST_ASSERT_GREATER_THAN(OOC_ERROR_STANDARD, OOC_ERROR_COUNT);
// }
//
// void test_core_memory_allocation_failure(void) {
//     // This is difficult to test without mocking malloc
//     // We can test edge cases that might trigger allocation failures
//     void* obj = ooc_new(ooc_objectClass());
//     TEST_ASSERT_NOT_NULL(obj);
//
//     // Test clone which allocates memory
//     void* clone = ooc_clone(obj);
//     TEST_ASSERT_NOT_NULL(clone);
//     TEST_ASSERT_NOT_EQUAL(obj, clone);
//
//     ooc_destroy(obj);
//     ooc_destroy(clone);
// }
//
// void test_class_creation_invalid_args(void) {
//     TestClass invalidClass;
//
//     // Test ooc_classNew with NULL class
//     TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT,
//                      ooc_classNew(NULL, "Invalid",
//                                  sizeof(TestObject), sizeof(TestClass),
//                                  ooc_objectClass(), OOC_MODIFIER_NONE, 0));
// }
//
// void test_class_inheritance_chain(void) {
//     void* testClass = getTestClass();
//     TEST_ASSERT_NOT_NULL(testClass);
//
//     // Test inheritance chain
//     TEST_ASSERT_EQUAL_PTR(ooc_objectClass(), ooc_classSuper(testClass));
//     TEST_ASSERT_NULL(ooc_classSuper(ooc_objectClass()));
//
//     // Create object and test inheritance
//     void* obj = ooc_new(testClass, 42, "test");
//     TEST_ASSERT_NOT_NULL(obj);
//
//     TEST_ASSERT_TRUE(ooc_isInstanceOf(obj, testClass));
//     TEST_ASSERT_TRUE(ooc_isInstanceOf(obj, ooc_objectClass()));
//
//     ooc_destroy(obj);
// }
//
// void test_class_method_inheritance(void) {
//     void* obj = ooc_new(getTestClass(), 42, "inherited");
//     TEST_ASSERT_NOT_NULL(obj);
//
//     // Test that inherited methods work
//     TEST_ASSERT_TRUE(ooc_equals(obj, obj));  // Inherited from Object
//     TEST_ASSERT_GREATER_THAN(0, ooc_hashCode(obj));  // Inherited from Object
//
//     // Test overridden method
//     const char* str = ooc_toString(obj);
//     TEST_ASSERT_NOT_NULL(str);
//     TEST_ASSERT_NOT_NULL(strstr(str, "TestObject"));
//     TEST_ASSERT_NOT_NULL(strstr(str, "value=42"));
//     TEST_ASSERT_NOT_NULL(strstr(str, "data=inherited"));
//
//     free((void*)str);
//     ooc_destroy(obj);
// }
//
// void test_class_final_inheritance_prevention(void) {
//     TestClass finalClass;
//
//     // Create a final class
//     OOC_Error result = ooc_classNew(&finalClass,
//                                    "FinalClass",
//                                    sizeof(TestObject),
//                                    sizeof(TestClass),
//                                    ooc_objectClass(),
//                                    OOC_MODIFIER_FINAL,
//                                    0);
//     TEST_ASSERT_EQUAL(OOC_ERROR_NONE, result);
//
//     // Try to inherit from final class - should fail
//     TestClass derivedClass;
//     result = ooc_classNew(&derivedClass,
//                          "DerivedFromFinal",
//                          sizeof(TestObject),
//                          sizeof(TestClass),
//                          &finalClass,
//                          OOC_MODIFIER_NONE,
//                          0);
//     TEST_ASSERT_EQUAL(OOC_ERROR_FINAL_CLASS_INHERITANCE, result);
// }
//
// void test_class_abstract_instantiation_prevention(void) {
//     TestClass abstractClass;
//
//     // Create an abstract class
//     OOC_Error result = ooc_classNew(&abstractClass,
//                                    "AbstractClass",
//                                    sizeof(TestObject),
//                                    sizeof(TestClass),
//                                    ooc_objectClass(),
//                                    OOC_MODIFIER_ABSTRACT,
//                                    0);
//     TEST_ASSERT_EQUAL(OOC_ERROR_NONE, result);
//
//     // Try to instantiate abstract class - should fail
//     void* obj = ooc_new(&abstractClass);
//     TEST_ASSERT_NULL(obj);
// }
//
// void test_class_introspection(void) {
//     void* testClass = getTestClass();
//
//     // Test class introspection
//     TEST_ASSERT_EQUAL_STRING("TestObject", ooc_classGetName(testClass));
//     TEST_ASSERT_EQUAL_UINT(sizeof(TestObject), ooc_classSizeOf(testClass));
//     TEST_ASSERT_EQUAL_UINT(sizeof(TestClass), ooc_classSelfSize(testClass));
//     TEST_ASSERT_FALSE(ooc_classIsAbstract(testClass));
//     TEST_ASSERT_FALSE(ooc_classIsFinal(testClass));
//
//     // Test object introspection
//     void* obj = ooc_new(testClass, 100, "introspection");
//     TEST_ASSERT_EQUAL_STRING("TestObject", ooc_getClassName(obj));
//     TEST_ASSERT_EQUAL_UINT(sizeof(TestObject), ooc_sizeOf(obj));
//     TEST_ASSERT_FALSE(ooc_isAbstract(obj));
//     TEST_ASSERT_FALSE(ooc_isFinal(obj));
//
//     ooc_destroy(obj);
// }
//
// void test_class_modifier_combinations(void) {
//     TestClass modifiedClass;
//
//     // Test FINAL modifier
//     OOC_Error result = ooc_classNew(&modifiedClass,
//                                    "FinalClass",
//                                    sizeof(TestObject),
//                                    sizeof(TestClass),
//                                    ooc_objectClass(),
//                                    OOC_MODIFIER_FINAL,
//                                    0);
//     TEST_ASSERT_EQUAL(OOC_ERROR_NONE, result);
//     TEST_ASSERT_TRUE(ooc_classIsFinal(&modifiedClass));
//     TEST_ASSERT_FALSE(ooc_classIsAbstract(&modifiedClass));
//
//     // Reset for next test
//     memset(&modifiedClass, 0, sizeof(modifiedClass));
//
//     // Test ABSTRACT modifier
//     result = ooc_classNew(&modifiedClass,
//                          "AbstractClass",
//                          sizeof(TestObject),
//                          sizeof(TestClass),
//                          ooc_objectClass(),
//                          OOC_MODIFIER_ABSTRACT,
//                          0);
//     TEST_ASSERT_EQUAL(OOC_ERROR_NONE, result);
//     TEST_ASSERT_FALSE(ooc_classIsFinal(&modifiedClass));
//     TEST_ASSERT_TRUE(ooc_classIsAbstract(&modifiedClass));
// }
//
// static OOC_Error failConstructor(void* self, va_list* args) {
//     return OOC_ERROR_OUT_OF_MEMORY;  // Simulate failure
// }
//
// void test_object_constructor_failure(void) {
//     // Test constructor that returns error
//     TestClass failClass;
//
//     OOC_Error result = ooc_classNew(&failClass,
//                                    "FailClass",
//                                    sizeof(TestObject),
//                                    sizeof(TestClass),
//                                    ooc_objectClass(),
//                                    OOC_MODIFIER_NONE,
//                                    OOC_METHOD_CTOR, failConstructor,
//                                    0);
//     TEST_ASSERT_EQUAL(OOC_ERROR_NONE, result);
//
//     // Object creation should fail
//     void* obj = ooc_new(&failClass);
//     TEST_ASSERT_NULL(obj);
// }
//
// void test_object_destructor_failure(void) {
//     TestClass failClass;
//
//      OOC_Error failDestructor(void* self) {
//         return OOC_ERROR_STANDARD;  // Simulate failure
//     }
//
//     OOC_Error result = ooc_classNew(&failClass,
//                                    "FailDestructorClass",
//                                    sizeof(TestObject),
//                                    sizeof(TestClass),
//                                    ooc_objectClass(),
//                                    OOC_MODIFIER_NONE,
//                                    OOC_METHOD_DTOR, failDestructor,
//                                    0);
//     TEST_ASSERT_EQUAL(OOC_ERROR_NONE, result);
//
//     void* obj = ooc_new(&failClass);
//     TEST_ASSERT_NOT_NULL(obj);
//
//     // Destruction should return error
//     TEST_ASSERT_EQUAL(OOC_ERROR_STANDARD, ooc_destroy(obj));
// }
//
// void test_object_double_destruction(void) {
//     void* obj = ooc_new(ooc_objectClass());
//     TEST_ASSERT_NOT_NULL(obj);
//
//     // First destruction should succeed
//     TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_destroy(obj));
//
//     // Double destruction of same memory is undefined behavior
//     // This test documents the current behavior but shouldn't be relied upon
// }
//
// void test_object_null_destruction(void) {
//     // NULL destruction should be safe
//     TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_destroy(NULL));
// }
//
// void test_method_dispatch(void) {
//     void* obj1 = ooc_new(ooc_objectClass());
//     void* obj2 = ooc_new(getTestClass(), 50, "dispatch");
//
//     // Test that different classes have different toString behavior
//     const char* str1 = ooc_toString(obj1);
//     const char* str2 = ooc_toString(obj2);
//
//     TEST_ASSERT_NOT_NULL(str1);
//     TEST_ASSERT_NOT_NULL(str2);
//     //TEST_ASSERT_NOT_EQUAL_STRING(str1, str2);
//
//     // Object toString should contain "Object"
//     TEST_ASSERT_NOT_NULL(strstr(str1, "Object"));
//
//     // TestObject toString should contain "TestObject" and "value=50"
//     TEST_ASSERT_NOT_NULL(strstr(str2, "TestObject"));
//     TEST_ASSERT_NOT_NULL(strstr(str2, "value=50"));
//
//     free((void*)str1);
//     free((void*)str2);
//     ooc_destroy(obj1);
//     ooc_destroy(obj2);
// }
//
// void test_virtual_method_calls(void) {
//     void* obj = ooc_new(getTestClass(), 75, "virtual");
//
//     // Cast to base class pointer and call virtual method
//     void* basePtr = obj;  // Simulate polymorphic call
//
//     const char* str = ooc_toString(basePtr);
//     TEST_ASSERT_NOT_NULL(str);
//
//     // Should call derived class method even through base pointer
//     TEST_ASSERT_NOT_NULL(strstr(str, "TestObject"));
//     TEST_ASSERT_NOT_NULL(strstr(str, "value=75"));
//
//     free((void*)str);
//     ooc_destroy(obj);
// }
//
// void test_super_method_calls(void) {
//     void* obj = ooc_new(getTestClass(), 25, "super");
//
//     // Test that we can access super class
//     void* superClass = ooc_super(obj);
//     TEST_ASSERT_EQUAL_PTR(ooc_objectClass(), superClass);
//
//     // Test inherited behavior
//     TEST_ASSERT_TRUE(ooc_equals(obj, obj));  // Uses inherited equals
//     TEST_ASSERT_GREATER_THAN(0, ooc_hashCode(obj));  // Uses inherited hash
//
//     ooc_destroy(obj);
// }
