#include "oocUInteger.h"
#include "oocUInteger.r"
#include "oocNumber.h"
#include "oocObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static OOC_UIntegerClass* UIntegerClass;
static OOC_UIntegerClass UIntegerClassInstance;

unsigned int ooc_uintegerGetValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_uintegerClass(), 0);
    const OOC_UInteger* entry = self;
    return entry->value;
}

static OOC_Error ooc_uintegerConstructor(void* self, va_list* args) {
    OOC_TYPE_CHECK(self, ooc_uintegerClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(ooc_uintegerClass(), self, args);
    if (error != OOC_ERROR_NONE) return error;
    OOC_UInteger* entry = self;
    entry->value = va_arg(*args, unsigned int);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_uintegerDestructor(void* self) {
    OOC_TYPE_CHECK(self, ooc_uintegerClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_superDtor(ooc_uintegerClass(), self);
}

static char* ooc_uintegerToString(const void* self) {
    OOC_TYPE_CHECK(self, ooc_uintegerClass(), NULL);
    const OOC_UInteger* entry = self;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%u", entry->value);
    char* result = malloc(strlen(buffer) + 1);
    if (result) strcpy(result, buffer);
    return result;
}

static bool ooc_uintegerEquals(const void* self, const void* other) {
    if (self == other) return true;
    if (!self || !other) return false;
    OOC_TYPE_CHECK(self, ooc_uintegerClass(), false);
    OOC_TYPE_CHECK(other, ooc_uintegerClass(), false);
    return ((const OOC_UInteger*)self)->value == ((const OOC_UInteger*)other)->value;
}

static size_t ooc_uintegerHash(const void* self) {
    OOC_TYPE_CHECK(self, ooc_uintegerClass(), 0);
    return (size_t)((const OOC_UInteger*)self)->value;
}

static int ooc_uintegerCompare(const void* self, const void* other) {
    OOC_TYPE_CHECK(self, ooc_uintegerClass(), 0);
    OOC_TYPE_CHECK(other, ooc_uintegerClass(), 0);
    unsigned int a = ((const OOC_UInteger*)self)->value;
    unsigned int b = ((const OOC_UInteger*)other)->value;
    return (a > b) - (a < b);
}

static void* ooc_uintegerClone(const void* self) {
    OOC_TYPE_CHECK(self, ooc_uintegerClass(), NULL);
    return ooc_new(ooc_uintegerClass(), ((const OOC_UInteger*)self)->value);
}

static int ooc_uintegerIntValue(const void* self) {
    return (int)((const OOC_UInteger*)self)->value;
}

static unsigned int ooc_uintegerUIntValue(const void* self) {
    return ((const OOC_UInteger*)self)->value;
}

static long ooc_uintegerLongValue(const void* self) {
    return (long)((const OOC_UInteger*)self)->value;
}

static unsigned long ooc_uintegerULongValue(const void* self) {
    return (unsigned long)((const OOC_UInteger*)self)->value;
}

static float ooc_uintegerFloatValue(const void* self) {
    return (float)((const OOC_UInteger*)self)->value;
}

static double ooc_uintegerDoubleValue(const void* self) {
    return (double)((const OOC_UInteger*)self)->value;
}

static void* ooc_uintegerClassInit(void) {
    if (ooc_classNew(&UIntegerClassInstance,
                    "UInteger",
                    sizeof(OOC_UInteger),
                    sizeof(OOC_UIntegerClass),
                    ooc_numberClass(),
                    OOC_MODIFIER_FINAL,
                    OOC_METHOD_CTOR, ooc_uintegerConstructor,
                    OOC_METHOD_DTOR, ooc_uintegerDestructor,
                    OOC_METHOD_TO_STRING, ooc_uintegerToString,
                    OOC_METHOD_EQUALS, ooc_uintegerEquals,
                    OOC_METHOD_HASH, ooc_uintegerHash,
                    OOC_METHOD_COMPARE, ooc_uintegerCompare,
                    OOC_METHOD_CLONE, ooc_uintegerClone,
                    OOC_NUMBER_METHOD_INT_VALUE, ooc_uintegerIntValue,
                    OOC_NUMBER_METHOD_UINT_VALUE, ooc_uintegerUIntValue,
                    OOC_NUMBER_METHOD_LONG_VALUE, ooc_uintegerLongValue,
                    OOC_NUMBER_METHOD_ULONG_VALUE, ooc_uintegerULongValue,
                    OOC_NUMBER_METHOD_FLOAT_VALUE, ooc_uintegerFloatValue,
                    OOC_NUMBER_METHOD_DOUBLE_VALUE, ooc_uintegerDoubleValue,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &UIntegerClassInstance;
}

void* ooc_uintegerClass(void) {
    if (!UIntegerClass) {
        UIntegerClass = ooc_uintegerClassInit();
    }
    return UIntegerClass;
}
