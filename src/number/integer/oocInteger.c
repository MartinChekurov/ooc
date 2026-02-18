#include "oocInteger.h"
#include "oocInteger.r"
#include "oocNumber.h"
#include "oocObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static OOC_IntegerClass* IntegerClass;
static OOC_IntegerClass IntegerClassInstance;

int ooc_integerGetValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_integerClass(), 0);
    const OOC_Integer* entry = self;
    return entry->value;
}

static OOC_Error ooc_integerConstructor(void* self, va_list* args) {
    OOC_TYPE_CHECK(self, ooc_integerClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(ooc_integerClass(), self, args);
    if (error != OOC_ERROR_NONE) return error;
    OOC_Integer* entry = self;
    entry->value = va_arg(*args, int);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_integerDestructor(void* self) {
    OOC_TYPE_CHECK(self, ooc_integerClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_superDtor(ooc_integerClass(), self);
}

static char* ooc_integerToString(const void* self) {
    OOC_TYPE_CHECK(self, ooc_integerClass(), NULL);
    const OOC_Integer* entry = self;
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", entry->value);
    char* result = malloc(strlen(buffer) + 1);
    if (result) strcpy(result, buffer);
    return result;
}

static bool ooc_integerEquals(const void* self, const void* other) {
    if (self == other) return true;
    if (!self || !other) return false;
    OOC_TYPE_CHECK(self, ooc_integerClass(), false);
    OOC_TYPE_CHECK(other, ooc_integerClass(), false);
    return ((const OOC_Integer*)self)->value == ((const OOC_Integer*)other)->value;
}

static size_t ooc_integerHash(const void* self) {
    OOC_TYPE_CHECK(self, ooc_integerClass(), 0);
    return (size_t)((const OOC_Integer*)self)->value;
}

static int ooc_integerCompare(const void* self, const void* other) {
    OOC_TYPE_CHECK(self, ooc_integerClass(), 0);
    OOC_TYPE_CHECK(other, ooc_integerClass(), 0);
    int a = ((const OOC_Integer*)self)->value;
    int b = ((const OOC_Integer*)other)->value;
    return (a > b) - (a < b);
}

static void* ooc_integerClone(const void* self) {
    OOC_TYPE_CHECK(self, ooc_integerClass(), NULL);
    return ooc_new(ooc_integerClass(), ((const OOC_Integer*)self)->value);
}

static int ooc_integerIntValue(const void* self) {
    return ((const OOC_Integer*)self)->value;
}

static unsigned int ooc_integerUIntValue(const void* self) {
    return (unsigned int)((const OOC_Integer*)self)->value;
}

static long ooc_integerLongValue(const void* self) {
    return (long)((const OOC_Integer*)self)->value;
}

static unsigned long ooc_integerULongValue(const void* self) {
    return (unsigned long)((const OOC_Integer*)self)->value;
}

static float ooc_integerFloatValue(const void* self) {
    return (float)((const OOC_Integer*)self)->value;
}

static double ooc_integerDoubleValue(const void* self) {
    return (double)((const OOC_Integer*)self)->value;
}

static void* ooc_integerClassInit(void) {
    if (ooc_classNew(&IntegerClassInstance,
                    "Integer",
                    sizeof(OOC_Integer),
                    sizeof(OOC_IntegerClass),
                    ooc_numberClass(),
                    OOC_MODIFIER_FINAL,
                    OOC_METHOD_CTOR, ooc_integerConstructor,
                    OOC_METHOD_DTOR, ooc_integerDestructor,
                    OOC_METHOD_TO_STRING, ooc_integerToString,
                    OOC_METHOD_EQUALS, ooc_integerEquals,
                    OOC_METHOD_HASH, ooc_integerHash,
                    OOC_METHOD_COMPARE, ooc_integerCompare,
                    OOC_METHOD_CLONE, ooc_integerClone,
                    OOC_NUMBER_METHOD_INT_VALUE, ooc_integerIntValue,
                    OOC_NUMBER_METHOD_UINT_VALUE, ooc_integerUIntValue,
                    OOC_NUMBER_METHOD_LONG_VALUE, ooc_integerLongValue,
                    OOC_NUMBER_METHOD_ULONG_VALUE, ooc_integerULongValue,
                    OOC_NUMBER_METHOD_FLOAT_VALUE, ooc_integerFloatValue,
                    OOC_NUMBER_METHOD_DOUBLE_VALUE, ooc_integerDoubleValue,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &IntegerClassInstance;
}

void* ooc_integerClass(void) {
    if (!IntegerClass) {
        IntegerClass = ooc_integerClassInit();
    }
    return IntegerClass;
}
