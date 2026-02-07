#include "unity.h"
#include "oocString.h"
#include "oocError.h"
#include <stdlib.h>
#include <string.h>

// void setUp(void) {
//     // Setup code here
// }
//
// void tearDown(void) {
//     // Cleanup code here
// }

void test_string_class_creation(void) {
    void* strClass = ooc_stringClass();
    TEST_ASSERT_NOT_NULL(strClass);
}

void test_string_from_int(void) {
    char* str = ooc_StringFromInt(42);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_EQUAL_STRING("42", str);
    free(str);

    str = ooc_StringFromInt(-123);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_EQUAL_STRING("-123", str);
    free(str);
}

void test_string_from_bool(void) {
    char* str = ooc_StringFromBool(true);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_EQUAL_STRING("true", str);
    free(str);

    str = ooc_StringFromBool(false);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_EQUAL_STRING("false", str);
    free(str);
}

void test_string_length(void) {
    TEST_ASSERT_EQUAL_UINT(5, ooc_StringLength("Hello"));
    TEST_ASSERT_EQUAL_UINT(0, ooc_StringLength(""));
    TEST_ASSERT_EQUAL_UINT(0, ooc_StringLength(NULL));
}

void test_string_char_at(void) {
    const char* str = "Hello";
    TEST_ASSERT_EQUAL_CHAR('H', ooc_StringCharAt(str, 0));
    TEST_ASSERT_EQUAL_CHAR('e', ooc_StringCharAt(str, 1));
    TEST_ASSERT_EQUAL_CHAR('o', ooc_StringCharAt(str, 4));
    TEST_ASSERT_EQUAL_CHAR('\0', ooc_StringCharAt(str, 5)); // Out of bounds
}

void test_string_substring(void) {
    const char* str = "Hello World";
    char* sub = ooc_StringSubstring(str, 0, 5);
    TEST_ASSERT_NOT_NULL(sub);
    TEST_ASSERT_EQUAL_STRING("Hello", sub);
    free(sub);

    sub = ooc_StringSubstring(str, 6, 11);
    TEST_ASSERT_NOT_NULL(sub);
    TEST_ASSERT_EQUAL_STRING("World", sub);
    free(sub);
}

void test_string_concat(void) {
    char* result = ooc_StringConcat("Hello", " World");
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_STRING("Hello World", result);
    free(result);
}

void test_string_starts_with(void) {
    TEST_ASSERT_TRUE(ooc_StringStartsWithString("Hello World", "Hello"));
    TEST_ASSERT_FALSE(ooc_StringStartsWithString("Hello World", "World"));
    TEST_ASSERT_TRUE(ooc_StringStartsWithString("Hello", ""));
}

void test_string_ends_with(void) {
    TEST_ASSERT_TRUE(ooc_StringEndsWithString("Hello World", "World"));
    TEST_ASSERT_FALSE(ooc_StringEndsWithString("Hello World", "Hello"));
    TEST_ASSERT_TRUE(ooc_StringEndsWithString("Hello", ""));
}

void test_string_index_of(void) {
    TEST_ASSERT_EQUAL_INT(1, ooc_StringIndexOfChar("Hello", 'e'));
    TEST_ASSERT_EQUAL_INT(-1, ooc_StringIndexOfChar("Hello", 'x'));
    TEST_ASSERT_EQUAL_INT(0, ooc_StringIndexOfChar("Hello", 'H'));
}

void test_string_is_empty(void) {
    TEST_ASSERT_TRUE(ooc_StringIsEmpty(""));
    TEST_ASSERT_TRUE(ooc_StringIsEmpty(NULL));
    TEST_ASSERT_FALSE(ooc_StringIsEmpty("Hello"));
}

void test_string_buffer_operations(void) {
    char buffer[100];

    OOC_Error result = ooc_StringFromIntAt(buffer, sizeof(buffer), 42);
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, result);
    TEST_ASSERT_EQUAL_STRING("42", buffer);

    result = ooc_StringConcatAt(buffer, sizeof(buffer), "Hello", " World");
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, result);
    TEST_ASSERT_EQUAL_STRING("Hello World", buffer);
}
