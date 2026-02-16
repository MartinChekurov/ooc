#include "unity.h"
#include "oocString.h"
#include "oocObject.h"
#include "oocError.h"
#include <stdlib.h>
#include <string.h>

void test_string_create_destroy(void) {
    void* str = ooc_new(ooc_stringClass(), "hello");
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_TRUE(ooc_isInstanceOf(str, ooc_stringClass()));
    ooc_destroy(str);
}

void test_string_length(void) {
    void* str = ooc_new(ooc_stringClass(), "hello");
    TEST_ASSERT_EQUAL(5, ooc_stringLength(str));
    ooc_destroy(str);

    void* empty = ooc_new(ooc_stringClass(), "");
    TEST_ASSERT_EQUAL(0, ooc_stringLength(empty));
    ooc_destroy(empty);
}

void test_string_char_at(void) {
    void* str = ooc_new(ooc_stringClass(), "hello");
    TEST_ASSERT_EQUAL_CHAR('h', ooc_stringCharAt(str, 0));
    TEST_ASSERT_EQUAL_CHAR('o', ooc_stringCharAt(str, 4));
    ooc_destroy(str);
}

void test_string_is_empty(void) {
    void* str = ooc_new(ooc_stringClass(), "hello");
    TEST_ASSERT_FALSE(ooc_stringIsEmpty(str));
    ooc_destroy(str);

    void* empty = ooc_new(ooc_stringClass(), "");
    TEST_ASSERT_TRUE(ooc_stringIsEmpty(empty));
    ooc_destroy(empty);
}

void test_string_substring(void) {
    void* str = ooc_new(ooc_stringClass(), "hello world");
    void* sub = ooc_stringSubstring(str, 0, 5);
    TEST_ASSERT_NOT_NULL(sub);
    const char* cstr = ooc_stringGetCString(sub);
    TEST_ASSERT_EQUAL_STRING("hello", cstr);
    ooc_destroy(sub);
    ooc_destroy(str);
}

void test_string_concat(void) {
    void* s1 = ooc_new(ooc_stringClass(), "hello");
    void* s2 = ooc_new(ooc_stringClass(), " world");
    void* result = ooc_stringConcat(s1, s2);
    TEST_ASSERT_NOT_NULL(result);
    const char* cstr = ooc_stringGetCString(result);
    TEST_ASSERT_EQUAL_STRING("hello world", cstr);
    ooc_destroy(result);
    ooc_destroy(s1);
    ooc_destroy(s2);
}

void test_string_starts_with(void) {
    void* str = ooc_new(ooc_stringClass(), "hello world");
    TEST_ASSERT_TRUE(ooc_stringStartsWithCString(str, "hello"));
    TEST_ASSERT_FALSE(ooc_stringStartsWithCString(str, "world"));

    void* prefix = ooc_new(ooc_stringClass(), "hello");
    TEST_ASSERT_TRUE(ooc_stringStartsWithString(str, prefix));
    ooc_destroy(prefix);
    ooc_destroy(str);
}

void test_string_ends_with(void) {
    void* str = ooc_new(ooc_stringClass(), "hello world");
    TEST_ASSERT_TRUE(ooc_stringEndsWithCString(str, "world"));
    TEST_ASSERT_FALSE(ooc_stringEndsWithCString(str, "hello"));

    void* suffix = ooc_new(ooc_stringClass(), "world");
    TEST_ASSERT_TRUE(ooc_stringEndsWithString(str, suffix));
    ooc_destroy(suffix);
    ooc_destroy(str);
}

void test_string_index_of_char(void) {
    void* str = ooc_new(ooc_stringClass(), "hello");
    TEST_ASSERT_EQUAL(1, ooc_stringIndexOfChar(str, 'e'));
    TEST_ASSERT_EQUAL(-1, ooc_stringIndexOfChar(str, 'z'));
    ooc_destroy(str);
}

void test_string_last_index_of_char(void) {
    void* str = ooc_new(ooc_stringClass(), "hello");
    TEST_ASSERT_EQUAL(3, ooc_stringLastIndexOfChar(str, 'l'));
    TEST_ASSERT_EQUAL(-1, ooc_stringLastIndexOfChar(str, 'z'));
    ooc_destroy(str);
}

void test_string_index_of_string(void) {
    void* str = ooc_new(ooc_stringClass(), "hello world");
    TEST_ASSERT_EQUAL(6, ooc_stringIndexOfCString(str, "world"));
    TEST_ASSERT_EQUAL(-1, ooc_stringIndexOfCString(str, "xyz"));
    ooc_destroy(str);
}

void test_string_replace_char(void) {
    void* str = ooc_new(ooc_stringClass(), "hello");
    void* result = ooc_stringReplaceChar(str, 'l', 'r');
    TEST_ASSERT_NOT_NULL(result);
    const char* cstr = ooc_stringGetCString(result);
    TEST_ASSERT_EQUAL_STRING("herro", cstr);
    ooc_destroy(result);
    ooc_destroy(str);
}

void test_string_to_lower_upper(void) {
    void* str = ooc_new(ooc_stringClass(), "Hello World");
    void* lower = ooc_stringToLowerCase(str);
    TEST_ASSERT_EQUAL_STRING("hello world", ooc_stringGetCString(lower));
    void* upper = ooc_stringToUpperCase(str);
    TEST_ASSERT_EQUAL_STRING("HELLO WORLD", ooc_stringGetCString(upper));
    ooc_destroy(upper);
    ooc_destroy(lower);
    ooc_destroy(str);
}

void test_string_trim(void) {
    void* str = ooc_new(ooc_stringClass(), "  hello  ");
    void* trimmed = ooc_stringTrim(str);
    TEST_ASSERT_EQUAL_STRING("hello", ooc_stringGetCString(trimmed));
    ooc_destroy(trimmed);
    ooc_destroy(str);
}

void test_string_contains(void) {
    void* str = ooc_new(ooc_stringClass(), "hello world");
    TEST_ASSERT_TRUE(ooc_stringContainsCString(str, "world"));
    TEST_ASSERT_FALSE(ooc_stringContainsCString(str, "xyz"));
    ooc_destroy(str);
}

void test_string_to_cstring(void) {
    void* str = ooc_new(ooc_stringClass(), "hello");
    char* cstr = ooc_stringToCString(str);
    TEST_ASSERT_EQUAL_STRING("hello", cstr);
    free(cstr);

    const char* ref = ooc_stringGetCString(str);
    TEST_ASSERT_EQUAL_STRING("hello", ref);
    ooc_destroy(str);
}

void test_string_equals_and_hash(void) {
    void* s1 = ooc_new(ooc_stringClass(), "hello");
    void* s2 = ooc_new(ooc_stringClass(), "hello");
    void* s3 = ooc_new(ooc_stringClass(), "world");

    TEST_ASSERT_TRUE(ooc_equals(s1, s2));
    TEST_ASSERT_FALSE(ooc_equals(s1, s3));
    TEST_ASSERT_EQUAL(ooc_hashCode(s1), ooc_hashCode(s2));

    ooc_destroy(s1);
    ooc_destroy(s2);
    ooc_destroy(s3);
}

void test_string_clone(void) {
    void* str = ooc_new(ooc_stringClass(), "hello");
    void* clone = ooc_clone(str);
    TEST_ASSERT_NOT_NULL(clone);
    TEST_ASSERT_TRUE(ooc_equals(str, clone));
    TEST_ASSERT_TRUE(str != clone);
    ooc_destroy(str);
    ooc_destroy(clone);
}

void test_string_static_from_int(void) {
    char* str = ooc_StringFromInt(42);
    TEST_ASSERT_EQUAL_STRING("42", str);
    free(str);

    char* neg = ooc_StringFromInt(-7);
    TEST_ASSERT_EQUAL_STRING("-7", neg);
    free(neg);
}

void test_string_static_from_bool(void) {
    char* t = ooc_StringFromBool(true);
    TEST_ASSERT_EQUAL_STRING("true", t);
    free(t);

    char* f = ooc_StringFromBool(false);
    TEST_ASSERT_EQUAL_STRING("false", f);
    free(f);
}

void test_string_static_length(void) {
    TEST_ASSERT_EQUAL(5, ooc_StringLength("hello"));
    TEST_ASSERT_EQUAL(0, ooc_StringLength(""));
}

void test_string_static_char_at(void) {
    TEST_ASSERT_EQUAL_CHAR('h', ooc_StringCharAt("hello", 0));
    TEST_ASSERT_EQUAL_CHAR('o', ooc_StringCharAt("hello", 4));
}

void test_string_static_substring(void) {
    char* sub = ooc_StringSubstring("hello world", 0, 5);
    TEST_ASSERT_EQUAL_STRING("hello", sub);
    free(sub);
}

void test_string_static_concat(void) {
    char* result = ooc_StringConcat("hello", " world");
    TEST_ASSERT_EQUAL_STRING("hello world", result);
    free(result);
}

void test_string_static_index_of(void) {
    TEST_ASSERT_EQUAL(1, ooc_StringIndexOfChar("hello", 'e'));
    TEST_ASSERT_EQUAL(-1, ooc_StringIndexOfChar("hello", 'z'));
    TEST_ASSERT_EQUAL(6, ooc_StringIndexOfString("hello world", "world"));
}

void test_string_static_format_and_convert_helpers(void) {
    char* formatted = ooc_StringFormat("%s:%d", "id", 17);
    TEST_ASSERT_EQUAL_STRING("id:17", formatted);
    free(formatted);

    char* dbl = ooc_StringFromDouble(12.5);
    TEST_ASSERT_EQUAL_STRING("12.5", dbl);
    free(dbl);

    char* chr = ooc_StringFromChar('Q');
    TEST_ASSERT_EQUAL_STRING("Q", chr);
    free(chr);

    TEST_ASSERT_NULL(ooc_StringFormat(NULL));
}

void test_string_static_buffer_operations(void) {
    char buffer[32] = {0};

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_StringFormatAt(buffer, sizeof(buffer), "%s-%d", "v", 4));
    TEST_ASSERT_EQUAL_STRING("v-4", buffer);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_StringFromDoubleAt(buffer, sizeof(buffer), 3.5));
    TEST_ASSERT_EQUAL_STRING("3.5", buffer);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_StringFromCharAt(buffer, sizeof(buffer), 'A'));
    TEST_ASSERT_EQUAL_STRING("A", buffer);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_StringSubstringAt(buffer, sizeof(buffer), "abcdef", 1, 4));
    TEST_ASSERT_EQUAL_STRING("bcd", buffer);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_StringConcatAt(buffer, sizeof(buffer), "ab", "cd"));
    TEST_ASSERT_EQUAL_STRING("abcd", buffer);

    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_StringFormatAt(NULL, sizeof(buffer), "%d", 1));
    TEST_ASSERT_EQUAL(OOC_ERROR_BUFFER_TOO_SMALL, ooc_StringConcatAt(buffer, 4, "ab", "cd"));
}

void test_string_static_mutating_helpers(void) {
    char mutableStr[32] = "  AabA  ";

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_StringSetCharAt(mutableStr, 2, 'Z'));
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_StringReplaceChar(mutableStr, 'A', 'x'));
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_StringToLowerCase(mutableStr));
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_StringToUpperCase(mutableStr));
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_StringReverse(mutableStr));
    TEST_ASSERT_EQUAL_STRING("  XBAZ  ", mutableStr);

    TEST_ASSERT_EQUAL_PTR(mutableStr, ooc_StringTrim(mutableStr));
    TEST_ASSERT_EQUAL_STRING("XBAZ", mutableStr);
    TEST_ASSERT_TRUE(ooc_StringStartsWithString(mutableStr, "XB"));
    TEST_ASSERT_TRUE(ooc_StringEndsWithString(mutableStr, "AZ"));
    TEST_ASSERT_TRUE(ooc_StringContainsString(mutableStr, "BA"));
    TEST_ASSERT_EQUAL(3, ooc_StringLastIndexOfChar("abca", 'a'));
    TEST_ASSERT_EQUAL(3, ooc_StringLastIndexOfString("abcabc", "abc"));

    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_StringSetCharAt(mutableStr, 99, 'X'));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_StringReplaceChar(NULL, 'a', 'b'));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_StringReverse(NULL));
}
