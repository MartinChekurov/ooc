#include "oocDouble.h"
#include "oocDouble.r"
#include "oocNumber.h"
#include "oocObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static OOC_DoubleClass* DoubleClass;
static OOC_DoubleClass DoubleClassInstance;

double ooc_doubleGetValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_doubleClass(), 0);
    const OOC_Double* entry = self;
    return entry->value;
}

static OOC_Error ooc_doubleConstructor(void* self, va_list* args) {
    OOC_TYPE_CHECK(self, ooc_doubleClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(ooc_doubleClass(), self, args);
    if (error != OOC_ERROR_NONE) return error;
    OOC_Double* entry = self;
    entry->value = va_arg(*args, double);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_doubleDestructor(void* self) {
    OOC_TYPE_CHECK(self, ooc_doubleClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_superDtor(ooc_doubleClass(), self);
}

static char* ooc_doubleToString(const void* self) {
    OOC_TYPE_CHECK(self, ooc_doubleClass(), NULL);
    const OOC_Double* entry = self;
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%g", entry->value);
    char* result = malloc(strlen(buffer) + 1);
    if (result) strcpy(result, buffer);
    return result;
}

static bool ooc_doubleEquals(const void* self, const void* other) {
    if (self == other) return true;
    if (!self || !other) return false;
    OOC_TYPE_CHECK(self, ooc_doubleClass(), false);
    OOC_TYPE_CHECK(other, ooc_doubleClass(), false);
    return ((const OOC_Double*)self)->value == ((const OOC_Double*)other)->value;
}

static size_t ooc_doubleHash(const void* self) {
    OOC_TYPE_CHECK(self, ooc_doubleClass(), 0);
    double v = ((const OOC_Double*)self)->value;
    if (v == 0.0) v = 0.0;
    size_t h = 0;
    memcpy(&h, &v, sizeof(v) < sizeof(h) ? sizeof(v) : sizeof(h));
    return h;
}

static int ooc_doubleCompare(const void* self, const void* other) {
    OOC_TYPE_CHECK(self, ooc_doubleClass(), 0);
    OOC_TYPE_CHECK(other, ooc_doubleClass(), 0);
    double a = ((const OOC_Double*)self)->value;
    double b = ((const OOC_Double*)other)->value;
    return (a > b) - (a < b);
}

static void* ooc_doubleClone(const void* self) {
    OOC_TYPE_CHECK(self, ooc_doubleClass(), NULL);
    return ooc_new(ooc_doubleClass(), ((const OOC_Double*)self)->value);
}

static int ooc_doubleIntValue(const void* self) {
    return (int)((const OOC_Double*)self)->value;
}

static unsigned int ooc_doubleUIntValue(const void* self) {
    return (unsigned int)((const OOC_Double*)self)->value;
}

static long ooc_doubleLongValue(const void* self) {
    return (long)((const OOC_Double*)self)->value;
}

static unsigned long ooc_doubleULongValue(const void* self) {
    return (unsigned long)((const OOC_Double*)self)->value;
}

static float ooc_doubleFloatValue(const void* self) {
    return (float)((const OOC_Double*)self)->value;
}

static double ooc_doubleDoubleValue(const void* self) {
    return ((const OOC_Double*)self)->value;
}

static void* ooc_doubleClassInit(void) {
    if (ooc_classNew(&DoubleClassInstance,
                    "Double",
                    sizeof(OOC_Double),
                    sizeof(OOC_DoubleClass),
                    ooc_numberClass(),
                    OOC_MODIFIER_FINAL,
                    OOC_METHOD_CTOR, ooc_doubleConstructor,
                    OOC_METHOD_DTOR, ooc_doubleDestructor,
                    OOC_METHOD_TO_STRING, ooc_doubleToString,
                    OOC_METHOD_EQUALS, ooc_doubleEquals,
                    OOC_METHOD_HASH, ooc_doubleHash,
                    OOC_METHOD_COMPARE, ooc_doubleCompare,
                    OOC_METHOD_CLONE, ooc_doubleClone,
                    OOC_NUMBER_METHOD_INT_VALUE, ooc_doubleIntValue,
                    OOC_NUMBER_METHOD_UINT_VALUE, ooc_doubleUIntValue,
                    OOC_NUMBER_METHOD_LONG_VALUE, ooc_doubleLongValue,
                    OOC_NUMBER_METHOD_ULONG_VALUE, ooc_doubleULongValue,
                    OOC_NUMBER_METHOD_FLOAT_VALUE, ooc_doubleFloatValue,
                    OOC_NUMBER_METHOD_DOUBLE_VALUE, ooc_doubleDoubleValue,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &DoubleClassInstance;
}

void* ooc_doubleClass(void) {
    if (!DoubleClass) {
        DoubleClass = ooc_doubleClassInit();
    }
    return DoubleClass;
}
