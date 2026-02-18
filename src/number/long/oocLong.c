#include "oocLong.h"
#include "oocLong.r"
#include "oocNumber.h"
#include "oocObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static OOC_LongClass* LongClass;
static OOC_LongClass LongClassInstance;

long ooc_longGetValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_longClass(), 0);
    const OOC_Long* entry = self;
    return entry->value;
}

static OOC_Error ooc_longConstructor(void* self, va_list* args) {
    OOC_TYPE_CHECK(self, ooc_longClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(ooc_longClass(), self, args);
    if (error != OOC_ERROR_NONE) return error;
    OOC_Long* entry = self;
    entry->value = va_arg(*args, long);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_longDestructor(void* self) {
    OOC_TYPE_CHECK(self, ooc_longClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_superDtor(ooc_longClass(), self);
}

static char* ooc_longToString(const void* self) {
    OOC_TYPE_CHECK(self, ooc_longClass(), NULL);
    const OOC_Long* entry = self;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%ld", entry->value);
    char* result = malloc(strlen(buffer) + 1);
    if (result) strcpy(result, buffer);
    return result;
}

static bool ooc_longEquals(const void* self, const void* other) {
    if (self == other) return true;
    if (!self || !other) return false;
    OOC_TYPE_CHECK(self, ooc_longClass(), false);
    OOC_TYPE_CHECK(other, ooc_longClass(), false);
    return ((const OOC_Long*)self)->value == ((const OOC_Long*)other)->value;
}

static size_t ooc_longHash(const void* self) {
    OOC_TYPE_CHECK(self, ooc_longClass(), 0);
    return (size_t)((const OOC_Long*)self)->value;
}

static int ooc_longCompare(const void* self, const void* other) {
    OOC_TYPE_CHECK(self, ooc_longClass(), 0);
    OOC_TYPE_CHECK(other, ooc_longClass(), 0);
    long a = ((const OOC_Long*)self)->value;
    long b = ((const OOC_Long*)other)->value;
    return (a > b) - (a < b);
}

static void* ooc_longClone(const void* self) {
    OOC_TYPE_CHECK(self, ooc_longClass(), NULL);
    return ooc_new(ooc_longClass(), ((const OOC_Long*)self)->value);
}

static int ooc_longIntValue(const void* self) {
    return (int)((const OOC_Long*)self)->value;
}

static unsigned int ooc_longUIntValue(const void* self) {
    return (unsigned int)((const OOC_Long*)self)->value;
}

static long ooc_longLongValue(const void* self) {
    return ((const OOC_Long*)self)->value;
}

static unsigned long ooc_longULongValue(const void* self) {
    return (unsigned long)((const OOC_Long*)self)->value;
}

static float ooc_longFloatValue(const void* self) {
    return (float)((const OOC_Long*)self)->value;
}

static double ooc_longDoubleValue(const void* self) {
    return (double)((const OOC_Long*)self)->value;
}

static void* ooc_longClassInit(void) {
    if (ooc_classNew(&LongClassInstance,
                    "Long",
                    sizeof(OOC_Long),
                    sizeof(OOC_LongClass),
                    ooc_numberClass(),
                    OOC_MODIFIER_FINAL,
                    OOC_METHOD_CTOR, ooc_longConstructor,
                    OOC_METHOD_DTOR, ooc_longDestructor,
                    OOC_METHOD_TO_STRING, ooc_longToString,
                    OOC_METHOD_EQUALS, ooc_longEquals,
                    OOC_METHOD_HASH, ooc_longHash,
                    OOC_METHOD_COMPARE, ooc_longCompare,
                    OOC_METHOD_CLONE, ooc_longClone,
                    OOC_NUMBER_METHOD_INT_VALUE, ooc_longIntValue,
                    OOC_NUMBER_METHOD_UINT_VALUE, ooc_longUIntValue,
                    OOC_NUMBER_METHOD_LONG_VALUE, ooc_longLongValue,
                    OOC_NUMBER_METHOD_ULONG_VALUE, ooc_longULongValue,
                    OOC_NUMBER_METHOD_FLOAT_VALUE, ooc_longFloatValue,
                    OOC_NUMBER_METHOD_DOUBLE_VALUE, ooc_longDoubleValue,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &LongClassInstance;
}

void* ooc_longClass(void) {
    if (!LongClass) {
        LongClass = ooc_longClassInit();
    }
    return LongClass;
}
