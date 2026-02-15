#include "unity.h"
#include "oocStringBuffer.h"
#include "oocObject.h"
#include "oocError.h"
#include <stdlib.h>
#include <string.h>

void test_string_buffer_create_destroy(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello");
    TEST_ASSERT_NOT_NULL(buf);
    TEST_ASSERT_TRUE(ooc_isInstanceOf(buf, ooc_stringBufferClass()));
    ooc_destroy(buf);

    void* empty = ooc_new(ooc_stringBufferClass(), NULL);
    TEST_ASSERT_NOT_NULL(empty);
    TEST_ASSERT_EQUAL(0, ooc_stringBufferLength(empty));
    ooc_destroy(empty);
}

void test_string_buffer_length_and_empty(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello");
    TEST_ASSERT_EQUAL(5, ooc_stringBufferLength(buf));
    TEST_ASSERT_FALSE(ooc_stringBufferIsEmpty(buf));
    ooc_destroy(buf);

    void* empty = ooc_new(ooc_stringBufferClass(), NULL);
    TEST_ASSERT_EQUAL(0, ooc_stringBufferLength(empty));
    TEST_ASSERT_TRUE(ooc_stringBufferIsEmpty(empty));
    ooc_destroy(empty);
}

void test_string_buffer_append_cstring(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello");
    ooc_stringBufferAppendCString(buf, " world");
    TEST_ASSERT_EQUAL_STRING("hello world", ooc_stringBufferGetCString(buf));
    ooc_destroy(buf);
}

void test_string_buffer_append_char(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "ab");
    ooc_stringBufferAppendChar(buf, 'c');
    TEST_ASSERT_EQUAL_STRING("abc", ooc_stringBufferGetCString(buf));
    ooc_destroy(buf);
}

void test_string_buffer_append_int(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "val=");
    ooc_stringBufferAppendInt(buf, 42);
    TEST_ASSERT_EQUAL_STRING("val=42", ooc_stringBufferGetCString(buf));
    ooc_destroy(buf);
}

void test_string_buffer_append_bool(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), NULL);
    ooc_stringBufferAppendBool(buf, true);
    TEST_ASSERT_EQUAL_STRING("true", ooc_stringBufferGetCString(buf));
    ooc_destroy(buf);
}

void test_string_buffer_char_at(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello");
    TEST_ASSERT_EQUAL_CHAR('h', ooc_stringBufferCharAt(buf, 0));
    TEST_ASSERT_EQUAL_CHAR('o', ooc_stringBufferCharAt(buf, 4));
    ooc_destroy(buf);
}

void test_string_buffer_set_char_at(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello");
    ooc_stringBufferSetCharAt(buf, 0, 'H');
    TEST_ASSERT_EQUAL_STRING("Hello", ooc_stringBufferGetCString(buf));
    ooc_destroy(buf);
}

void test_string_buffer_substring(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello world");
    char* sub = ooc_stringBufferSubstring(buf, 0, 5);
    TEST_ASSERT_EQUAL_STRING("hello", sub);
    free(sub);
    ooc_destroy(buf);
}

void test_string_buffer_insert_cstring(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "helo");
    ooc_stringBufferInsertCString(buf, 2, "ll");
    TEST_ASSERT_EQUAL_STRING("hello", ooc_stringBufferGetCString(buf));
    ooc_destroy(buf);
}

void test_string_buffer_delete(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello world");
    ooc_stringBufferDelete(buf, 5, 11);
    TEST_ASSERT_EQUAL_STRING("hello", ooc_stringBufferGetCString(buf));
    ooc_destroy(buf);
}

void test_string_buffer_delete_char_at(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello");
    ooc_stringBufferDeleteCharAt(buf, 1);
    TEST_ASSERT_EQUAL_STRING("hllo", ooc_stringBufferGetCString(buf));
    ooc_destroy(buf);
}

void test_string_buffer_replace_cstring(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello world");
    ooc_stringBufferReplaceCString(buf, "world", "there");
    TEST_ASSERT_EQUAL_STRING("hello there", ooc_stringBufferGetCString(buf));
    ooc_destroy(buf);
}

void test_string_buffer_index_of(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello");
    TEST_ASSERT_EQUAL(1, ooc_stringBufferIndexOfChar(buf, 'e'));
    TEST_ASSERT_EQUAL(-1, ooc_stringBufferIndexOfChar(buf, 'z'));
    TEST_ASSERT_EQUAL(3, ooc_stringBufferLastIndexOfChar(buf, 'l'));
    ooc_destroy(buf);
}

void test_string_buffer_starts_ends_contains(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello world");
    TEST_ASSERT_TRUE(ooc_stringBufferStartsWithCString(buf, "hello"));
    TEST_ASSERT_FALSE(ooc_stringBufferStartsWithCString(buf, "world"));
    TEST_ASSERT_TRUE(ooc_stringBufferEndsWithCString(buf, "world"));
    TEST_ASSERT_TRUE(ooc_stringBufferContainsCString(buf, "lo wo"));
    ooc_destroy(buf);
}

void test_string_buffer_to_cstring(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello");
    char* cstr = ooc_stringBufferToCString(buf);
    TEST_ASSERT_EQUAL_STRING("hello", cstr);
    free(cstr);
    ooc_destroy(buf);
}

void test_string_buffer_clear(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello");
    ooc_stringBufferClear(buf);
    TEST_ASSERT_EQUAL(0, ooc_stringBufferLength(buf));
    TEST_ASSERT_TRUE(ooc_stringBufferIsEmpty(buf));
    ooc_destroy(buf);
}

void test_string_buffer_reverse(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "hello");
    ooc_stringBufferReverse(buf);
    TEST_ASSERT_EQUAL_STRING("olleh", ooc_stringBufferGetCString(buf));
    ooc_destroy(buf);
}

void test_string_buffer_trim(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "  hello  ");
    ooc_stringBufferTrim(buf);
    TEST_ASSERT_EQUAL_STRING("hello", ooc_stringBufferGetCString(buf));
    ooc_destroy(buf);
}

void test_string_buffer_case(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), "Hello");
    ooc_stringBufferToLowerCase(buf);
    TEST_ASSERT_EQUAL_STRING("hello", ooc_stringBufferGetCString(buf));
    ooc_stringBufferToUpperCase(buf);
    TEST_ASSERT_EQUAL_STRING("HELLO", ooc_stringBufferGetCString(buf));
    ooc_destroy(buf);
}

void test_string_buffer_capacity(void) {
    void* buf = ooc_new(ooc_stringBufferClass(), NULL);
    size_t cap = ooc_stringBufferCapacity(buf);
    TEST_ASSERT_TRUE(cap > 0);
    ooc_stringBufferEnsureCapacity(buf, 100);
    TEST_ASSERT_TRUE(ooc_stringBufferCapacity(buf) >= 100);
    ooc_stringBufferAppendCString(buf, "hi");
    ooc_stringBufferTrimToSize(buf);
    TEST_ASSERT_EQUAL(ooc_stringBufferLength(buf), ooc_stringBufferCapacity(buf));
    ooc_destroy(buf);
}
