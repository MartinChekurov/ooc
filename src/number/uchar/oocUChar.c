#include "oocUChar.h"
#include "oocUChar.r"
#include "oocNumber.h"
#include "oocObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static OOC_UCharClass* UCharClass;
static OOC_UCharClass UCharClassInstance;

unsigned char ooc_ucharGetValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_ucharClass(), 0);
    const OOC_UChar* entry = self;
    return entry->value;
}

static OOC_Error ooc_ucharConstructor(void* self, va_list* args) {
    OOC_TYPE_CHECK(self, ooc_ucharClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(ooc_ucharClass(), self, args);
    if (error != OOC_ERROR_NONE) return error;
    OOC_UChar* entry = self;
    entry->value = (unsigned char)va_arg(*args, int);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_ucharDestructor(void* self) {
    OOC_TYPE_CHECK(self, ooc_ucharClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_superDtor(ooc_ucharClass(), self);
}

static char* ooc_ucharToString(const void* self) {
    OOC_TYPE_CHECK(self, ooc_ucharClass(), NULL);
    const OOC_UChar* entry = self;
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%u", (unsigned int)entry->value);
    char* result = malloc(strlen(buffer) + 1);
    if (result) strcpy(result, buffer);
    return result;
}

static bool ooc_ucharEquals(const void* self, const void* other) {
    if (self == other) return true;
    if (!self || !other) return false;
    OOC_TYPE_CHECK(self, ooc_ucharClass(), false);
    OOC_TYPE_CHECK(other, ooc_ucharClass(), false);
    return ((const OOC_UChar*)self)->value == ((const OOC_UChar*)other)->value;
}

static size_t ooc_ucharHash(const void* self) {
    OOC_TYPE_CHECK(self, ooc_ucharClass(), 0);
    return (size_t)((const OOC_UChar*)self)->value;
}

static int ooc_ucharCompare(const void* self, const void* other) {
    OOC_TYPE_CHECK(self, ooc_ucharClass(), 0);
    OOC_TYPE_CHECK(other, ooc_ucharClass(), 0);
    unsigned char a = ((const OOC_UChar*)self)->value;
    unsigned char b = ((const OOC_UChar*)other)->value;
    return (a > b) - (a < b);
}

static void* ooc_ucharClone(const void* self) {
    OOC_TYPE_CHECK(self, ooc_ucharClass(), NULL);
    return ooc_new(ooc_ucharClass(), ((const OOC_UChar*)self)->value);
}

static int ooc_ucharIntValue(const void* self) {
    return (int)((const OOC_UChar*)self)->value;
}

static unsigned int ooc_ucharUIntValue(const void* self) {
    return (unsigned int)((const OOC_UChar*)self)->value;
}

static long ooc_ucharLongValue(const void* self) {
    return (long)((const OOC_UChar*)self)->value;
}

static unsigned long ooc_ucharULongValue(const void* self) {
    return (unsigned long)((const OOC_UChar*)self)->value;
}

static float ooc_ucharFloatValue(const void* self) {
    return (float)((const OOC_UChar*)self)->value;
}

static double ooc_ucharDoubleValue(const void* self) {
    return (double)((const OOC_UChar*)self)->value;
}

static void* ooc_ucharClassInit(void) {
    if (ooc_classNew(&UCharClassInstance,
                    "UChar",
                    sizeof(OOC_UChar),
                    sizeof(OOC_UCharClass),
                    ooc_numberClass(),
                    OOC_MODIFIER_FINAL,
                    OOC_METHOD_CTOR, ooc_ucharConstructor,
                    OOC_METHOD_DTOR, ooc_ucharDestructor,
                    OOC_METHOD_TO_STRING, ooc_ucharToString,
                    OOC_METHOD_EQUALS, ooc_ucharEquals,
                    OOC_METHOD_HASH, ooc_ucharHash,
                    OOC_METHOD_COMPARE, ooc_ucharCompare,
                    OOC_METHOD_CLONE, ooc_ucharClone,
                    OOC_NUMBER_METHOD_INT_VALUE, ooc_ucharIntValue,
                    OOC_NUMBER_METHOD_UINT_VALUE, ooc_ucharUIntValue,
                    OOC_NUMBER_METHOD_LONG_VALUE, ooc_ucharLongValue,
                    OOC_NUMBER_METHOD_ULONG_VALUE, ooc_ucharULongValue,
                    OOC_NUMBER_METHOD_FLOAT_VALUE, ooc_ucharFloatValue,
                    OOC_NUMBER_METHOD_DOUBLE_VALUE, ooc_ucharDoubleValue,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &UCharClassInstance;
}

void* ooc_ucharClass(void) {
    if (!UCharClass) {
        UCharClass = ooc_ucharClassInit();
    }
    return UCharClass;
}
