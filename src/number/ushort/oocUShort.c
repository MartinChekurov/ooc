#include "oocUShort.h"
#include "oocUShort.r"
#include "oocNumber.h"
#include "oocObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static OOC_UShortClass* UShortClass;
static OOC_UShortClass UShortClassInstance;

unsigned short ooc_ushortGetValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_ushortClass(), 0);
    const OOC_UShort* entry = self;
    return entry->value;
}

static OOC_Error ooc_ushortConstructor(void* self, va_list* args) {
    OOC_TYPE_CHECK(self, ooc_ushortClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(ooc_ushortClass(), self, args);
    if (error != OOC_ERROR_NONE) return error;
    OOC_UShort* entry = self;
    entry->value = (unsigned short)va_arg(*args, int);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_ushortDestructor(void* self) {
    OOC_TYPE_CHECK(self, ooc_ushortClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_superDtor(ooc_ushortClass(), self);
}

static char* ooc_ushortToString(const void* self) {
    OOC_TYPE_CHECK(self, ooc_ushortClass(), NULL);
    const OOC_UShort* entry = self;
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%u", (unsigned int)entry->value);
    char* result = malloc(strlen(buffer) + 1);
    if (result) strcpy(result, buffer);
    return result;
}

static bool ooc_ushortEquals(const void* self, const void* other) {
    if (self == other) return true;
    if (!self || !other) return false;
    OOC_TYPE_CHECK(self, ooc_ushortClass(), false);
    OOC_TYPE_CHECK(other, ooc_ushortClass(), false);
    return ((const OOC_UShort*)self)->value == ((const OOC_UShort*)other)->value;
}

static size_t ooc_ushortHash(const void* self) {
    OOC_TYPE_CHECK(self, ooc_ushortClass(), 0);
    return (size_t)((const OOC_UShort*)self)->value;
}

static int ooc_ushortCompare(const void* self, const void* other) {
    OOC_TYPE_CHECK(self, ooc_ushortClass(), 0);
    OOC_TYPE_CHECK(other, ooc_ushortClass(), 0);
    unsigned short a = ((const OOC_UShort*)self)->value;
    unsigned short b = ((const OOC_UShort*)other)->value;
    return (a > b) - (a < b);
}

static void* ooc_ushortClone(const void* self) {
    OOC_TYPE_CHECK(self, ooc_ushortClass(), NULL);
    return ooc_new(ooc_ushortClass(), ((const OOC_UShort*)self)->value);
}

static int ooc_ushortIntValue(const void* self) {
    return (int)((const OOC_UShort*)self)->value;
}

static unsigned int ooc_ushortUIntValue(const void* self) {
    return (unsigned int)((const OOC_UShort*)self)->value;
}

static long ooc_ushortLongValue(const void* self) {
    return (long)((const OOC_UShort*)self)->value;
}

static unsigned long ooc_ushortULongValue(const void* self) {
    return (unsigned long)((const OOC_UShort*)self)->value;
}

static float ooc_ushortFloatValue(const void* self) {
    return (float)((const OOC_UShort*)self)->value;
}

static double ooc_ushortDoubleValue(const void* self) {
    return (double)((const OOC_UShort*)self)->value;
}

static void* ooc_ushortClassInit(void) {
    if (ooc_classNew(&UShortClassInstance,
                    "UShort",
                    sizeof(OOC_UShort),
                    sizeof(OOC_UShortClass),
                    ooc_numberClass(),
                    OOC_MODIFIER_FINAL,
                    OOC_METHOD_CTOR, ooc_ushortConstructor,
                    OOC_METHOD_DTOR, ooc_ushortDestructor,
                    OOC_METHOD_TO_STRING, ooc_ushortToString,
                    OOC_METHOD_EQUALS, ooc_ushortEquals,
                    OOC_METHOD_HASH, ooc_ushortHash,
                    OOC_METHOD_COMPARE, ooc_ushortCompare,
                    OOC_METHOD_CLONE, ooc_ushortClone,
                    OOC_NUMBER_METHOD_INT_VALUE, ooc_ushortIntValue,
                    OOC_NUMBER_METHOD_UINT_VALUE, ooc_ushortUIntValue,
                    OOC_NUMBER_METHOD_LONG_VALUE, ooc_ushortLongValue,
                    OOC_NUMBER_METHOD_ULONG_VALUE, ooc_ushortULongValue,
                    OOC_NUMBER_METHOD_FLOAT_VALUE, ooc_ushortFloatValue,
                    OOC_NUMBER_METHOD_DOUBLE_VALUE, ooc_ushortDoubleValue,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &UShortClassInstance;
}

void* ooc_ushortClass(void) {
    if (!UShortClass) {
        UShortClass = ooc_ushortClassInit();
    }
    return UShortClass;
}
