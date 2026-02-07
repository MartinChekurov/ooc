#include "unity.h"
#include "testObject.h"
#include "testString.h"
#include "testInterface.h"
#include "oocError.h"
#include "oocObject.h"

void setUp(void) {
    // Called before each test - add any setup code here
    // OOC_Error   ooc_classDestroy   (void* class);
    // ooc_classDestroy(ooc_objectClass());
}

void tearDown(void) {
    // Called after each test - add any cleanup code here
}

int main(void) {
    UNITY_BEGIN();

    // Object tests
    printf("Running Object Tests...\n");
    RUN_TEST(test_object_class_creation);
    RUN_TEST(test_object_creation_and_destruction);
    RUN_TEST(test_object_class_info);
    RUN_TEST(test_object_instanceof);
    RUN_TEST(test_object_equality);
    RUN_TEST(test_object_hash_code);
    RUN_TEST(test_object_to_string);
    RUN_TEST(test_object_clone);

    // Interface tests
    printf("\nRunning Interface Tests...\n");
    RUN_TEST(test_interface_class_creation);
    RUN_TEST(test_interface_instantiation);
    RUN_TEST(test_interface_constructor);
    RUN_TEST(test_interface_implementation);
    RUN_TEST(test_interface_multiple_implementations);
    RUN_TEST(test_interface_inheritance);
    RUN_TEST(test_interface_binding);
    RUN_TEST(test_interface_polymorphism);

    // String tests
    printf("\nRunning String Tests...\n");
    RUN_TEST(test_string_class_creation);
    RUN_TEST(test_string_from_int);
    RUN_TEST(test_string_from_bool);
    RUN_TEST(test_string_length);
    RUN_TEST(test_string_char_at);
    RUN_TEST(test_string_substring);
    RUN_TEST(test_string_concat);
    RUN_TEST(test_string_starts_with);
    RUN_TEST(test_string_ends_with);
    RUN_TEST(test_string_index_of);
    RUN_TEST(test_string_is_empty);
    RUN_TEST(test_string_buffer_operations);

    return UNITY_END();
}
