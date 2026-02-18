#include "oocFloat.h"
#include "oocFloat.r"
#include "oocNumber.h"
#include "oocObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static OOC_FloatClass* FloatClass;
static OOC_FloatClass FloatClassInstance;

float ooc_floatGetValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_floatClass(), 0);
    const OOC_Float* entry = self;
    return entry->value;
}

static OOC_Error ooc_floatConstructor(void* self, va_list* args) {
    OOC_TYPE_CHECK(self, ooc_floatClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(ooc_floatClass(), self, args);
    if (error != OOC_ERROR_NONE) return error;
    OOC_Float* entry = self;
    entry->value = (float)va_arg(*args, double);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_floatDestructor(void* self) {
    OOC_TYPE_CHECK(self, ooc_floatClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_superDtor(ooc_floatClass(), self);
}

static char* ooc_floatToString(const void* self) {
    OOC_TYPE_CHECK(self, ooc_floatClass(), NULL);
    const OOC_Float* entry = self;
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%g", (double)entry->value);
    char* result = malloc(strlen(buffer) + 1);
    if (result) strcpy(result, buffer);
    return result;
}

static bool ooc_floatEquals(const void* self, const void* other) {
    if (self == other) return true;
    if (!self || !other) return false;
    OOC_TYPE_CHECK(self, ooc_floatClass(), false);
    OOC_TYPE_CHECK(other, ooc_floatClass(), false);
    return ((const OOC_Float*)self)->value == ((const OOC_Float*)other)->value;
}

static size_t ooc_floatHash(const void* self) {
    OOC_TYPE_CHECK(self, ooc_floatClass(), 0);
    float v = ((const OOC_Float*)self)->value;
    if (v == 0.0f) v = 0.0f;
    size_t h = 0;
    memcpy(&h, &v, sizeof(v) < sizeof(h) ? sizeof(v) : sizeof(h));
    return h;
}

static int ooc_floatCompare(const void* self, const void* other) {
    OOC_TYPE_CHECK(self, ooc_floatClass(), 0);
    OOC_TYPE_CHECK(other, ooc_floatClass(), 0);
    float a = ((const OOC_Float*)self)->value;
    float b = ((const OOC_Float*)other)->value;
    return (a > b) - (a < b);
}

static void* ooc_floatClone(const void* self) {
    OOC_TYPE_CHECK(self, ooc_floatClass(), NULL);
    return ooc_new(ooc_floatClass(), (double)((const OOC_Float*)self)->value);
}

static int ooc_floatIntValue(const void* self) {
    return (int)((const OOC_Float*)self)->value;
}

static unsigned int ooc_floatUIntValue(const void* self) {
    return (unsigned int)((const OOC_Float*)self)->value;
}

static long ooc_floatLongValue(const void* self) {
    return (long)((const OOC_Float*)self)->value;
}

static unsigned long ooc_floatULongValue(const void* self) {
    return (unsigned long)((const OOC_Float*)self)->value;
}

static float ooc_floatFloatValue(const void* self) {
    return ((const OOC_Float*)self)->value;
}

static double ooc_floatDoubleValue(const void* self) {
    return (double)((const OOC_Float*)self)->value;
}

static void* ooc_floatClassInit(void) {
    if (ooc_classNew(&FloatClassInstance,
                    "Float",
                    sizeof(OOC_Float),
                    sizeof(OOC_FloatClass),
                    ooc_numberClass(),
                    OOC_MODIFIER_FINAL,
                    OOC_METHOD_CTOR, ooc_floatConstructor,
                    OOC_METHOD_DTOR, ooc_floatDestructor,
                    OOC_METHOD_TO_STRING, ooc_floatToString,
                    OOC_METHOD_EQUALS, ooc_floatEquals,
                    OOC_METHOD_HASH, ooc_floatHash,
                    OOC_METHOD_COMPARE, ooc_floatCompare,
                    OOC_METHOD_CLONE, ooc_floatClone,
                    OOC_NUMBER_METHOD_INT_VALUE, ooc_floatIntValue,
                    OOC_NUMBER_METHOD_UINT_VALUE, ooc_floatUIntValue,
                    OOC_NUMBER_METHOD_LONG_VALUE, ooc_floatLongValue,
                    OOC_NUMBER_METHOD_ULONG_VALUE, ooc_floatULongValue,
                    OOC_NUMBER_METHOD_FLOAT_VALUE, ooc_floatFloatValue,
                    OOC_NUMBER_METHOD_DOUBLE_VALUE, ooc_floatDoubleValue,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &FloatClassInstance;
}

void* ooc_floatClass(void) {
    if (!FloatClass) {
        FloatClass = ooc_floatClassInit();
    }
    return FloatClass;
}
