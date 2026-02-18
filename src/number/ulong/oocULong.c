#include "oocULong.h"
#include "oocULong.r"
#include "oocNumber.h"
#include "oocObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static OOC_ULongClass* ULongClass;
static OOC_ULongClass ULongClassInstance;

unsigned long ooc_ulongGetValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_ulongClass(), 0);
    const OOC_ULong* entry = self;
    return entry->value;
}

static OOC_Error ooc_ulongConstructor(void* self, va_list* args) {
    OOC_TYPE_CHECK(self, ooc_ulongClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(ooc_ulongClass(), self, args);
    if (error != OOC_ERROR_NONE) return error;
    OOC_ULong* entry = self;
    entry->value = va_arg(*args, unsigned long);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_ulongDestructor(void* self) {
    OOC_TYPE_CHECK(self, ooc_ulongClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_superDtor(ooc_ulongClass(), self);
}

static char* ooc_ulongToString(const void* self) {
    OOC_TYPE_CHECK(self, ooc_ulongClass(), NULL);
    const OOC_ULong* entry = self;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%lu", entry->value);
    char* result = malloc(strlen(buffer) + 1);
    if (result) strcpy(result, buffer);
    return result;
}

static bool ooc_ulongEquals(const void* self, const void* other) {
    if (self == other) return true;
    if (!self || !other) return false;
    OOC_TYPE_CHECK(self, ooc_ulongClass(), false);
    OOC_TYPE_CHECK(other, ooc_ulongClass(), false);
    return ((const OOC_ULong*)self)->value == ((const OOC_ULong*)other)->value;
}

static size_t ooc_ulongHash(const void* self) {
    OOC_TYPE_CHECK(self, ooc_ulongClass(), 0);
    return (size_t)((const OOC_ULong*)self)->value;
}

static int ooc_ulongCompare(const void* self, const void* other) {
    OOC_TYPE_CHECK(self, ooc_ulongClass(), 0);
    OOC_TYPE_CHECK(other, ooc_ulongClass(), 0);
    unsigned long a = ((const OOC_ULong*)self)->value;
    unsigned long b = ((const OOC_ULong*)other)->value;
    return (a > b) - (a < b);
}

static void* ooc_ulongClone(const void* self) {
    OOC_TYPE_CHECK(self, ooc_ulongClass(), NULL);
    return ooc_new(ooc_ulongClass(), ((const OOC_ULong*)self)->value);
}

static int ooc_ulongIntValue(const void* self) {
    return (int)((const OOC_ULong*)self)->value;
}

static unsigned int ooc_ulongUIntValue(const void* self) {
    return (unsigned int)((const OOC_ULong*)self)->value;
}

static long ooc_ulongLongValue(const void* self) {
    return (long)((const OOC_ULong*)self)->value;
}

static unsigned long ooc_ulongULongValue(const void* self) {
    return ((const OOC_ULong*)self)->value;
}

static float ooc_ulongFloatValue(const void* self) {
    return (float)((const OOC_ULong*)self)->value;
}

static double ooc_ulongDoubleValue(const void* self) {
    return (double)((const OOC_ULong*)self)->value;
}

static void* ooc_ulongClassInit(void) {
    if (ooc_classNew(&ULongClassInstance,
                    "ULong",
                    sizeof(OOC_ULong),
                    sizeof(OOC_ULongClass),
                    ooc_numberClass(),
                    OOC_MODIFIER_FINAL,
                    OOC_METHOD_CTOR, ooc_ulongConstructor,
                    OOC_METHOD_DTOR, ooc_ulongDestructor,
                    OOC_METHOD_TO_STRING, ooc_ulongToString,
                    OOC_METHOD_EQUALS, ooc_ulongEquals,
                    OOC_METHOD_HASH, ooc_ulongHash,
                    OOC_METHOD_COMPARE, ooc_ulongCompare,
                    OOC_METHOD_CLONE, ooc_ulongClone,
                    OOC_NUMBER_METHOD_INT_VALUE, ooc_ulongIntValue,
                    OOC_NUMBER_METHOD_UINT_VALUE, ooc_ulongUIntValue,
                    OOC_NUMBER_METHOD_LONG_VALUE, ooc_ulongLongValue,
                    OOC_NUMBER_METHOD_ULONG_VALUE, ooc_ulongULongValue,
                    OOC_NUMBER_METHOD_FLOAT_VALUE, ooc_ulongFloatValue,
                    OOC_NUMBER_METHOD_DOUBLE_VALUE, ooc_ulongDoubleValue,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &ULongClassInstance;
}

void* ooc_ulongClass(void) {
    if (!ULongClass) {
        ULongClass = ooc_ulongClassInit();
    }
    return ULongClass;
}
