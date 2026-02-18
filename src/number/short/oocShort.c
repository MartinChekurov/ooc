#include "oocShort.h"
#include "oocShort.r"
#include "oocNumber.h"
#include "oocObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static OOC_ShortClass* ShortClass;
static OOC_ShortClass ShortClassInstance;

short ooc_shortGetValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_shortClass(), 0);
    const OOC_Short* entry = self;
    return entry->value;
}

static OOC_Error ooc_shortConstructor(void* self, va_list* args) {
    OOC_TYPE_CHECK(self, ooc_shortClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(ooc_shortClass(), self, args);
    if (error != OOC_ERROR_NONE) return error;
    OOC_Short* entry = self;
    entry->value = (short)va_arg(*args, int);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_shortDestructor(void* self) {
    OOC_TYPE_CHECK(self, ooc_shortClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_superDtor(ooc_shortClass(), self);
}

static char* ooc_shortToString(const void* self) {
    OOC_TYPE_CHECK(self, ooc_shortClass(), NULL);
    const OOC_Short* entry = self;
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d", (int)entry->value);
    char* result = malloc(strlen(buffer) + 1);
    if (result) strcpy(result, buffer);
    return result;
}

static bool ooc_shortEquals(const void* self, const void* other) {
    if (self == other) return true;
    if (!self || !other) return false;
    OOC_TYPE_CHECK(self, ooc_shortClass(), false);
    OOC_TYPE_CHECK(other, ooc_shortClass(), false);
    return ((const OOC_Short*)self)->value == ((const OOC_Short*)other)->value;
}

static size_t ooc_shortHash(const void* self) {
    OOC_TYPE_CHECK(self, ooc_shortClass(), 0);
    return (size_t)((const OOC_Short*)self)->value;
}

static int ooc_shortCompare(const void* self, const void* other) {
    OOC_TYPE_CHECK(self, ooc_shortClass(), 0);
    OOC_TYPE_CHECK(other, ooc_shortClass(), 0);
    short a = ((const OOC_Short*)self)->value;
    short b = ((const OOC_Short*)other)->value;
    return (a > b) - (a < b);
}

static void* ooc_shortClone(const void* self) {
    OOC_TYPE_CHECK(self, ooc_shortClass(), NULL);
    return ooc_new(ooc_shortClass(), ((const OOC_Short*)self)->value);
}

static int ooc_shortIntValue(const void* self) {
    return (int)((const OOC_Short*)self)->value;
}

static unsigned int ooc_shortUIntValue(const void* self) {
    return (unsigned int)((const OOC_Short*)self)->value;
}

static long ooc_shortLongValue(const void* self) {
    return (long)((const OOC_Short*)self)->value;
}

static unsigned long ooc_shortULongValue(const void* self) {
    return (unsigned long)((const OOC_Short*)self)->value;
}

static float ooc_shortFloatValue(const void* self) {
    return (float)((const OOC_Short*)self)->value;
}

static double ooc_shortDoubleValue(const void* self) {
    return (double)((const OOC_Short*)self)->value;
}

static void* ooc_shortClassInit(void) {
    if (ooc_classNew(&ShortClassInstance,
                    "Short",
                    sizeof(OOC_Short),
                    sizeof(OOC_ShortClass),
                    ooc_numberClass(),
                    OOC_MODIFIER_FINAL,
                    OOC_METHOD_CTOR, ooc_shortConstructor,
                    OOC_METHOD_DTOR, ooc_shortDestructor,
                    OOC_METHOD_TO_STRING, ooc_shortToString,
                    OOC_METHOD_EQUALS, ooc_shortEquals,
                    OOC_METHOD_HASH, ooc_shortHash,
                    OOC_METHOD_COMPARE, ooc_shortCompare,
                    OOC_METHOD_CLONE, ooc_shortClone,
                    OOC_NUMBER_METHOD_INT_VALUE, ooc_shortIntValue,
                    OOC_NUMBER_METHOD_UINT_VALUE, ooc_shortUIntValue,
                    OOC_NUMBER_METHOD_LONG_VALUE, ooc_shortLongValue,
                    OOC_NUMBER_METHOD_ULONG_VALUE, ooc_shortULongValue,
                    OOC_NUMBER_METHOD_FLOAT_VALUE, ooc_shortFloatValue,
                    OOC_NUMBER_METHOD_DOUBLE_VALUE, ooc_shortDoubleValue,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &ShortClassInstance;
}

void* ooc_shortClass(void) {
    if (!ShortClass) {
        ShortClass = ooc_shortClassInit();
    }
    return ShortClass;
}
