#include "unity.h"
#include "oocNumber.h"
#include "oocChar.h"
#include "oocUChar.h"
#include "oocShort.h"
#include "oocUShort.h"
#include "oocInteger.h"
#include "oocUInteger.h"
#include "oocLong.h"
#include "oocULong.h"
#include "oocFloat.h"
#include "oocDouble.h"
#include "oocObject.h"
#include <stdlib.h>
#include <string.h>

/* ===== Char ===== */

void test_number_char(void) {
    void* c = ooc_new(ooc_charClass(), (int)'A');
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_TRUE(ooc_isInstanceOf(c, ooc_charClass()));
    TEST_ASSERT_EQUAL('A', ooc_charGetValue(c));

    /* Conversion methods */
    TEST_ASSERT_EQUAL_INT(65, ooc_numberIntValue(c));
    TEST_ASSERT_EQUAL_UINT(65, ooc_numberUIntValue(c));
    TEST_ASSERT_EQUAL(65L, ooc_numberLongValue(c));
    TEST_ASSERT_EQUAL(65UL, ooc_numberULongValue(c));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 65.0f, ooc_numberFloatValue(c));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 65.0f, (float)ooc_numberDoubleValue(c));

    /* equals / hash */
    void* c2 = ooc_new(ooc_charClass(), (int)'A');
    void* c3 = ooc_new(ooc_charClass(), (int)'B');
    TEST_ASSERT_TRUE(ooc_equals(c, c2));
    TEST_ASSERT_FALSE(ooc_equals(c, c3));
    TEST_ASSERT_EQUAL(ooc_hashCode(c), ooc_hashCode(c2));

    /* toString */
    char* s = ooc_toString(c);
    TEST_ASSERT_EQUAL_STRING("65", s);
    free(s);

    /* clone */
    void* cl = ooc_clone(c);
    TEST_ASSERT_TRUE(ooc_equals(c, cl));
    TEST_ASSERT_FALSE(c == cl);

    /* compare */
    TEST_ASSERT_EQUAL_INT(0, ooc_compare(c, c2));
    TEST_ASSERT_TRUE(ooc_compare(c, c3) < 0);
    TEST_ASSERT_TRUE(ooc_compare(c3, c) > 0);

    ooc_destroy(c);
    ooc_destroy(c2);
    ooc_destroy(c3);
    ooc_destroy(cl);
}

/* ===== UChar ===== */

void test_number_uchar(void) {
    void* c = ooc_new(ooc_ucharClass(), (int)200);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL(200, ooc_ucharGetValue(c));

    TEST_ASSERT_EQUAL_INT(200, ooc_numberIntValue(c));
    TEST_ASSERT_EQUAL_UINT(200, ooc_numberUIntValue(c));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 200.0f, (float)ooc_numberDoubleValue(c));

    void* c2 = ooc_new(ooc_ucharClass(), (int)200);
    void* c3 = ooc_new(ooc_ucharClass(), (int)100);
    TEST_ASSERT_TRUE(ooc_equals(c, c2));
    TEST_ASSERT_FALSE(ooc_equals(c, c3));
    TEST_ASSERT_EQUAL(ooc_hashCode(c), ooc_hashCode(c2));

    char* s = ooc_toString(c);
    TEST_ASSERT_EQUAL_STRING("200", s);
    free(s);

    void* cl = ooc_clone(c);
    TEST_ASSERT_TRUE(ooc_equals(c, cl));
    TEST_ASSERT_FALSE(c == cl);

    TEST_ASSERT_EQUAL_INT(0, ooc_compare(c, c2));
    TEST_ASSERT_TRUE(ooc_compare(c3, c) < 0);

    ooc_destroy(c);
    ooc_destroy(c2);
    ooc_destroy(c3);
    ooc_destroy(cl);
}

/* ===== Short ===== */

void test_number_short(void) {
    void* n = ooc_new(ooc_shortClass(), (int)-1234);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL(-1234, ooc_shortGetValue(n));

    TEST_ASSERT_EQUAL_INT(-1234, ooc_numberIntValue(n));
    TEST_ASSERT_EQUAL(-1234L, ooc_numberLongValue(n));
    TEST_ASSERT_FLOAT_WITHIN(1.0f, -1234.0f, (float)ooc_numberDoubleValue(n));

    void* n2 = ooc_new(ooc_shortClass(), (int)-1234);
    void* n3 = ooc_new(ooc_shortClass(), (int)5678);
    TEST_ASSERT_TRUE(ooc_equals(n, n2));
    TEST_ASSERT_FALSE(ooc_equals(n, n3));
    TEST_ASSERT_EQUAL(ooc_hashCode(n), ooc_hashCode(n2));

    char* s = ooc_toString(n);
    TEST_ASSERT_EQUAL_STRING("-1234", s);
    free(s);

    void* cl = ooc_clone(n);
    TEST_ASSERT_TRUE(ooc_equals(n, cl));
    TEST_ASSERT_FALSE(n == cl);

    TEST_ASSERT_TRUE(ooc_compare(n, n3) < 0);

    ooc_destroy(n);
    ooc_destroy(n2);
    ooc_destroy(n3);
    ooc_destroy(cl);
}

/* ===== UShort ===== */

void test_number_ushort(void) {
    void* n = ooc_new(ooc_ushortClass(), (int)50000);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL(50000, ooc_ushortGetValue(n));

    TEST_ASSERT_EQUAL_INT(50000, ooc_numberIntValue(n));
    TEST_ASSERT_EQUAL_UINT(50000, ooc_numberUIntValue(n));

    void* n2 = ooc_new(ooc_ushortClass(), (int)50000);
    void* n3 = ooc_new(ooc_ushortClass(), (int)10000);
    TEST_ASSERT_TRUE(ooc_equals(n, n2));
    TEST_ASSERT_FALSE(ooc_equals(n, n3));

    char* s = ooc_toString(n);
    TEST_ASSERT_EQUAL_STRING("50000", s);
    free(s);

    void* cl = ooc_clone(n);
    TEST_ASSERT_TRUE(ooc_equals(n, cl));
    TEST_ASSERT_FALSE(n == cl);

    TEST_ASSERT_TRUE(ooc_compare(n3, n) < 0);

    ooc_destroy(n);
    ooc_destroy(n2);
    ooc_destroy(n3);
    ooc_destroy(cl);
}

/* ===== Integer ===== */

void test_number_integer(void) {
    void* n = ooc_new(ooc_integerClass(), 42);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL(42, ooc_integerGetValue(n));

    TEST_ASSERT_EQUAL_INT(42, ooc_numberIntValue(n));
    TEST_ASSERT_EQUAL_UINT(42, ooc_numberUIntValue(n));
    TEST_ASSERT_EQUAL(42L, ooc_numberLongValue(n));
    TEST_ASSERT_EQUAL(42UL, ooc_numberULongValue(n));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 42.0f, ooc_numberFloatValue(n));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 42.0f, (float)ooc_numberDoubleValue(n));

    void* n2 = ooc_new(ooc_integerClass(), 42);
    void* n3 = ooc_new(ooc_integerClass(), 99);
    TEST_ASSERT_TRUE(ooc_equals(n, n2));
    TEST_ASSERT_FALSE(ooc_equals(n, n3));
    TEST_ASSERT_EQUAL(ooc_hashCode(n), ooc_hashCode(n2));

    char* s = ooc_toString(n);
    TEST_ASSERT_EQUAL_STRING("42", s);
    free(s);

    void* cl = ooc_clone(n);
    TEST_ASSERT_TRUE(ooc_equals(n, cl));
    TEST_ASSERT_FALSE(n == cl);

    TEST_ASSERT_EQUAL_INT(0, ooc_compare(n, n2));
    TEST_ASSERT_TRUE(ooc_compare(n, n3) < 0);
    TEST_ASSERT_TRUE(ooc_compare(n3, n) > 0);

    /* Cross-type equals should fail */
    void* d = ooc_new(ooc_doubleClass(), 42.0);
    TEST_ASSERT_FALSE(ooc_equals(n, d));
    ooc_destroy(d);

    ooc_destroy(n);
    ooc_destroy(n2);
    ooc_destroy(n3);
    ooc_destroy(cl);
}

/* ===== UInteger ===== */

void test_number_uinteger(void) {
    void* n = ooc_new(ooc_uintegerClass(), 3000000000U);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL_UINT(3000000000U, ooc_uintegerGetValue(n));

    TEST_ASSERT_EQUAL_UINT(3000000000U, ooc_numberUIntValue(n));
    TEST_ASSERT_FLOAT_WITHIN(1024.0f, 3000000000.0f, (float)ooc_numberDoubleValue(n));

    void* n2 = ooc_new(ooc_uintegerClass(), 3000000000U);
    void* n3 = ooc_new(ooc_uintegerClass(), 100U);
    TEST_ASSERT_TRUE(ooc_equals(n, n2));
    TEST_ASSERT_FALSE(ooc_equals(n, n3));

    char* s = ooc_toString(n);
    TEST_ASSERT_EQUAL_STRING("3000000000", s);
    free(s);

    void* cl = ooc_clone(n);
    TEST_ASSERT_TRUE(ooc_equals(n, cl));
    TEST_ASSERT_FALSE(n == cl);

    TEST_ASSERT_TRUE(ooc_compare(n3, n) < 0);

    ooc_destroy(n);
    ooc_destroy(n2);
    ooc_destroy(n3);
    ooc_destroy(cl);
}

/* ===== Long ===== */

void test_number_long(void) {
    void* n = ooc_new(ooc_longClass(), 123456789L);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL(123456789L, ooc_longGetValue(n));

    TEST_ASSERT_EQUAL(123456789L, ooc_numberLongValue(n));
    TEST_ASSERT_FLOAT_WITHIN(16.0f, 123456789.0f, (float)ooc_numberDoubleValue(n));

    void* n2 = ooc_new(ooc_longClass(), 123456789L);
    void* n3 = ooc_new(ooc_longClass(), -1L);
    TEST_ASSERT_TRUE(ooc_equals(n, n2));
    TEST_ASSERT_FALSE(ooc_equals(n, n3));

    char* s = ooc_toString(n);
    TEST_ASSERT_EQUAL_STRING("123456789", s);
    free(s);

    void* cl = ooc_clone(n);
    TEST_ASSERT_TRUE(ooc_equals(n, cl));
    TEST_ASSERT_FALSE(n == cl);

    TEST_ASSERT_TRUE(ooc_compare(n3, n) < 0);

    ooc_destroy(n);
    ooc_destroy(n2);
    ooc_destroy(n3);
    ooc_destroy(cl);
}

/* ===== ULong ===== */

void test_number_ulong(void) {
    void* n = ooc_new(ooc_ulongClass(), 4000000000UL);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL(4000000000UL, ooc_ulongGetValue(n));

    TEST_ASSERT_EQUAL(4000000000UL, ooc_numberULongValue(n));

    void* n2 = ooc_new(ooc_ulongClass(), 4000000000UL);
    void* n3 = ooc_new(ooc_ulongClass(), 0UL);
    TEST_ASSERT_TRUE(ooc_equals(n, n2));
    TEST_ASSERT_FALSE(ooc_equals(n, n3));

    char* s = ooc_toString(n);
    TEST_ASSERT_EQUAL_STRING("4000000000", s);
    free(s);

    void* cl = ooc_clone(n);
    TEST_ASSERT_TRUE(ooc_equals(n, cl));
    TEST_ASSERT_FALSE(n == cl);

    TEST_ASSERT_TRUE(ooc_compare(n3, n) < 0);

    ooc_destroy(n);
    ooc_destroy(n2);
    ooc_destroy(n3);
    ooc_destroy(cl);
}

/* ===== Float ===== */

void test_number_float(void) {
    void* n = ooc_new(ooc_floatClass(), (double)3.14f);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.14f, ooc_floatGetValue(n));

    TEST_ASSERT_EQUAL_INT(3, ooc_numberIntValue(n));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.14f, ooc_numberFloatValue(n));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.14f, (float)ooc_numberDoubleValue(n));

    void* n2 = ooc_new(ooc_floatClass(), (double)3.14f);
    void* n3 = ooc_new(ooc_floatClass(), (double)2.71f);
    TEST_ASSERT_TRUE(ooc_equals(n, n2));
    TEST_ASSERT_FALSE(ooc_equals(n, n3));
    TEST_ASSERT_EQUAL(ooc_hashCode(n), ooc_hashCode(n2));

    char* s = ooc_toString(n);
    TEST_ASSERT_NOT_NULL(s);
    free(s);

    void* cl = ooc_clone(n);
    TEST_ASSERT_TRUE(ooc_equals(n, cl));
    TEST_ASSERT_FALSE(n == cl);

    TEST_ASSERT_TRUE(ooc_compare(n3, n) < 0);
    TEST_ASSERT_TRUE(ooc_compare(n, n3) > 0);
    TEST_ASSERT_EQUAL_INT(0, ooc_compare(n, n2));

    ooc_destroy(n);
    ooc_destroy(n2);
    ooc_destroy(n3);
    ooc_destroy(cl);
}

/* ===== Double ===== */

void test_number_double(void) {
    void* n = ooc_new(ooc_doubleClass(), 3.14159);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.14159f, (float)ooc_doubleGetValue(n));

    TEST_ASSERT_EQUAL_INT(3, ooc_numberIntValue(n));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 3.14159f, (float)ooc_numberDoubleValue(n));

    void* n2 = ooc_new(ooc_doubleClass(), 3.14159);
    void* n3 = ooc_new(ooc_doubleClass(), 2.71828);
    TEST_ASSERT_TRUE(ooc_equals(n, n2));
    TEST_ASSERT_FALSE(ooc_equals(n, n3));
    TEST_ASSERT_EQUAL(ooc_hashCode(n), ooc_hashCode(n2));

    char* s = ooc_toString(n);
    TEST_ASSERT_NOT_NULL(s);
    free(s);

    void* cl = ooc_clone(n);
    TEST_ASSERT_TRUE(ooc_equals(n, cl));
    TEST_ASSERT_FALSE(n == cl);

    TEST_ASSERT_TRUE(ooc_compare(n3, n) < 0);
    TEST_ASSERT_TRUE(ooc_compare(n, n3) > 0);
    TEST_ASSERT_EQUAL_INT(0, ooc_compare(n, n2));

    ooc_destroy(n);
    ooc_destroy(n2);
    ooc_destroy(n3);
    ooc_destroy(cl);
}

/* ===== instanceof Number ===== */

void test_number_instanceof(void) {
    void* i = ooc_new(ooc_integerClass(), 1);
    void* d = ooc_new(ooc_doubleClass(), 1.0);
    void* f = ooc_new(ooc_floatClass(), 1.0);
    void* l = ooc_new(ooc_longClass(), 1L);
    void* ul = ooc_new(ooc_ulongClass(), 1UL);
    void* c = ooc_new(ooc_charClass(), (int)'x');
    void* uc = ooc_new(ooc_ucharClass(), (int)1);
    void* s = ooc_new(ooc_shortClass(), (int)1);
    void* us = ooc_new(ooc_ushortClass(), (int)1);
    void* ui = ooc_new(ooc_uintegerClass(), 1U);

    TEST_ASSERT_TRUE(ooc_isInstanceOf(i, ooc_numberClass()));
    TEST_ASSERT_TRUE(ooc_isInstanceOf(d, ooc_numberClass()));
    TEST_ASSERT_TRUE(ooc_isInstanceOf(f, ooc_numberClass()));
    TEST_ASSERT_TRUE(ooc_isInstanceOf(l, ooc_numberClass()));
    TEST_ASSERT_TRUE(ooc_isInstanceOf(ul, ooc_numberClass()));
    TEST_ASSERT_TRUE(ooc_isInstanceOf(c, ooc_numberClass()));
    TEST_ASSERT_TRUE(ooc_isInstanceOf(uc, ooc_numberClass()));
    TEST_ASSERT_TRUE(ooc_isInstanceOf(s, ooc_numberClass()));
    TEST_ASSERT_TRUE(ooc_isInstanceOf(us, ooc_numberClass()));
    TEST_ASSERT_TRUE(ooc_isInstanceOf(ui, ooc_numberClass()));

    /* All are also instances of Object */
    TEST_ASSERT_TRUE(ooc_isInstanceOf(i, ooc_objectClass()));

    ooc_destroy(i);
    ooc_destroy(d);
    ooc_destroy(f);
    ooc_destroy(l);
    ooc_destroy(ul);
    ooc_destroy(c);
    ooc_destroy(uc);
    ooc_destroy(s);
    ooc_destroy(us);
    ooc_destroy(ui);
}
