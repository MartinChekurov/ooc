#include "oocChar.h"
#include "oocChar.r"
#include "oocNumber.h"
#include "oocObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static OOC_CharClass* CharClass;
static OOC_CharClass CharClassInstance;

char ooc_charGetValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_charClass(), 0);
    const OOC_Char* entry = self;
    return entry->value;
}

static OOC_Error ooc_charConstructor(void* self, va_list* args) {
    OOC_TYPE_CHECK(self, ooc_charClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(ooc_charClass(), self, args);
    if (error != OOC_ERROR_NONE) return error;
    OOC_Char* entry = self;
    entry->value = (char)va_arg(*args, int);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_charDestructor(void* self) {
    OOC_TYPE_CHECK(self, ooc_charClass(), OOC_ERROR_INVALID_OBJECT);
    return ooc_superDtor(ooc_charClass(), self);
}

static char* ooc_charToString(const void* self) {
    OOC_TYPE_CHECK(self, ooc_charClass(), NULL);
    const OOC_Char* entry = self;
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d", (int)entry->value);
    char* result = malloc(strlen(buffer) + 1);
    if (result) strcpy(result, buffer);
    return result;
}

static bool ooc_charEquals(const void* self, const void* other) {
    if (self == other) return true;
    if (!self || !other) return false;
    OOC_TYPE_CHECK(self, ooc_charClass(), false);
    OOC_TYPE_CHECK(other, ooc_charClass(), false);
    return ((const OOC_Char*)self)->value == ((const OOC_Char*)other)->value;
}

static size_t ooc_charHash(const void* self) {
    OOC_TYPE_CHECK(self, ooc_charClass(), 0);
    return (size_t)((const OOC_Char*)self)->value;
}

static int ooc_charCompare(const void* self, const void* other) {
    OOC_TYPE_CHECK(self, ooc_charClass(), 0);
    OOC_TYPE_CHECK(other, ooc_charClass(), 0);
    char a = ((const OOC_Char*)self)->value;
    char b = ((const OOC_Char*)other)->value;
    return (a > b) - (a < b);
}

static void* ooc_charClone(const void* self) {
    OOC_TYPE_CHECK(self, ooc_charClass(), NULL);
    return ooc_new(ooc_charClass(), ((const OOC_Char*)self)->value);
}

static int ooc_charIntValue(const void* self) {
    return (int)((const OOC_Char*)self)->value;
}

static unsigned int ooc_charUIntValue(const void* self) {
    return (unsigned int)((const OOC_Char*)self)->value;
}

static long ooc_charLongValue(const void* self) {
    return (long)((const OOC_Char*)self)->value;
}

static unsigned long ooc_charULongValue(const void* self) {
    return (unsigned long)((const OOC_Char*)self)->value;
}

static float ooc_charFloatValue(const void* self) {
    return (float)((const OOC_Char*)self)->value;
}

static double ooc_charDoubleValue(const void* self) {
    return (double)((const OOC_Char*)self)->value;
}

static void* ooc_charClassInit(void) {
    if (ooc_classNew(&CharClassInstance,
                    "Char",
                    sizeof(OOC_Char),
                    sizeof(OOC_CharClass),
                    ooc_numberClass(),
                    OOC_MODIFIER_FINAL,
                    OOC_METHOD_CTOR, ooc_charConstructor,
                    OOC_METHOD_DTOR, ooc_charDestructor,
                    OOC_METHOD_TO_STRING, ooc_charToString,
                    OOC_METHOD_EQUALS, ooc_charEquals,
                    OOC_METHOD_HASH, ooc_charHash,
                    OOC_METHOD_COMPARE, ooc_charCompare,
                    OOC_METHOD_CLONE, ooc_charClone,
                    OOC_NUMBER_METHOD_INT_VALUE, ooc_charIntValue,
                    OOC_NUMBER_METHOD_UINT_VALUE, ooc_charUIntValue,
                    OOC_NUMBER_METHOD_LONG_VALUE, ooc_charLongValue,
                    OOC_NUMBER_METHOD_ULONG_VALUE, ooc_charULongValue,
                    OOC_NUMBER_METHOD_FLOAT_VALUE, ooc_charFloatValue,
                    OOC_NUMBER_METHOD_DOUBLE_VALUE, ooc_charDoubleValue,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &CharClassInstance;
}

void* ooc_charClass(void) {
    if (!CharClass) {
        CharClass = ooc_charClassInit();
    }
    return CharClass;
}
