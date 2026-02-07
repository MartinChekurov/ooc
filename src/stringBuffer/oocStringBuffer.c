#include "oocStringBuffer.h"
#include "oocStringBuffer.r"
#include "oocString.h"
#include "oocObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEFAULT_CAPACITY 16

static OOC_StringBufferClass* StringBufferClass;
static OOC_StringBufferClass StringBufferClassInstance;

static OOC_Error ooc_stringBufferConstructor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(self, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    OOC_StringBuffer* buffer = self;
    const char* str = va_arg(*args, const char*);
    if (str) {
        buffer->length = strlen(str);
        buffer->capacity = buffer->length + DEFAULT_CAPACITY;
        buffer->data = malloc(buffer->capacity);
        if (!buffer->data) {
            return OOC_ERROR_OUT_OF_MEMORY;
        }
        strcpy(buffer->data, str);
    } else {
        buffer->length = 0;
        buffer->capacity = DEFAULT_CAPACITY;
        buffer->data = malloc(buffer->capacity);
        if (!buffer->data) {
            return OOC_ERROR_OUT_OF_MEMORY;
        }
        buffer->data[0] = '\0';
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_stringBufferDestructor(void* self) {
    if (!self) {
        return OOC_ERROR_NONE;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_StringBuffer* buffer = self;
    if (buffer->data) {
        free(buffer->data);
        buffer->data = NULL;
    }
    buffer->length = 0;
    buffer->capacity = 0;
    return ooc_superDtor(self);
}

static char* ooc_stringBufferToString_(const void* self) {
    return ooc_stringBufferToCString(self);
}

static bool ooc_stringBufferEquals(const void* self, const void* other) {
    if (!self || !other) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), false);
    OOC_TYPE_CHECK(other, ooc_stringBufferClass(), false);
    const OOC_StringBuffer* str1 = self;
    const OOC_StringBuffer* str2 = other;
    return strcmp(str1->data, str2->data) == 0;
}

static int ooc_stringBufferCompare(const void* self, const void* other) {
    if (!self || !other) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), -1);
    OOC_TYPE_CHECK(other, ooc_stringBufferClass(), -1);
    const OOC_StringBuffer* str1 = self;
    const OOC_StringBuffer* str2 = other;
    return strcmp(str1->data, str2->data);
}

static void* ooc_stringBufferClone(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    const OOC_StringBuffer* buffer = self;
    return ooc_new(ooc_stringBufferClass(), buffer->data);
}

static void* ooc_stringBufferClassInit(void) {
    if (ooc_classNew(&StringBufferClassInstance,
                    "StringBuffer",
                    sizeof(OOC_StringBuffer),
                    sizeof(OOC_StringBufferClass),
                    ooc_objectClass(),
                    OOC_MODIFIER_FINAL,
                    OOC_METHOD_CTOR, ooc_stringBufferConstructor,
                    OOC_METHOD_DTOR, ooc_stringBufferDestructor,
                    OOC_METHOD_TO_STRING, ooc_stringBufferToString_,
                    OOC_METHOD_EQUALS, ooc_stringBufferEquals,
                    OOC_METHOD_COMPARE, ooc_stringBufferCompare,
                    OOC_METHOD_CLONE, ooc_stringBufferClone,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &StringBufferClassInstance;
}

void* ooc_stringBufferClass() {
    if (!StringBufferClass) {
        StringBufferClass = ooc_stringBufferClassInit();
    }
    return StringBufferClass;
}

size_t ooc_stringBufferLength(const void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), 0);
    const OOC_StringBuffer* buffer = self;
    return buffer->length;
}

OOC_Error ooc_stringBufferSetLength(void* self, size_t newLength) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_StringBuffer* buffer = self;
    if (newLength > buffer->length) {
        if (ooc_stringBufferEnsureCapacity(self, newLength + 1) != OOC_ERROR_NONE) {
            return OOC_ERROR_OUT_OF_MEMORY;
        }
    }
    buffer->length = newLength;
    buffer->data[newLength] = '\0';
    return OOC_ERROR_NONE;
}

bool ooc_stringBufferIsEmpty(const void* self) {
    if (!self) {
        return true;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), true);
    const OOC_StringBuffer* buffer = self;
    return buffer->length == 0;
}

size_t ooc_stringBufferCapacity(const void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), 0);
    const OOC_StringBuffer* buffer = self;
    return buffer->capacity;
}

OOC_Error ooc_stringBufferEnsureCapacity(void* self, size_t minimumCapacity) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_StringBuffer* buffer = self;
    if (buffer->capacity >= minimumCapacity) {
        return OOC_ERROR_NONE;
    }
    size_t newCapacity = buffer->capacity * 2;
    if (newCapacity < minimumCapacity) {
        newCapacity = minimumCapacity;
    }
    char* newData = realloc(buffer->data, newCapacity);
    if (!newData) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    buffer->data = newData;
    buffer->capacity = newCapacity;
    return OOC_ERROR_NONE;
}

char ooc_stringBufferCharAt(const void* self, size_t index) {
    if (!self) {
        return '\0';
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), '\0');
    const OOC_StringBuffer* buffer = self;
    return ooc_StringCharAt(buffer->data, index);
}

OOC_Error ooc_stringBufferSetCharAt(void* self, size_t index, char ch) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_StringBuffer* buffer = self;
    return ooc_StringSetCharAt(buffer->data, index, ch);
}

char* ooc_stringBufferSubstring(const void* self, size_t start, size_t end) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    const OOC_StringBuffer* buffer = self;
    return ooc_StringSubstring(buffer->data, start, end);
}

void* ooc_stringBufferConcat(void* self, const void* other) {
    if (!self || !other) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_TYPE_CHECK(other, ooc_stringBufferClass(), NULL);
    return ooc_stringBufferAppendCString(self, ooc_stringBufferGetCString(other));
}

void* ooc_stringBufferReplaceWithString(void* self, size_t start, size_t end, const void* string) {
    return ooc_stringBufferReplaceWithCString(self, start, end, ooc_stringGetCString(string));
}

void* ooc_stringBufferReplaceWithCString(void* self, size_t start, size_t end, const char* str) {
    if (!self || !str) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_StringBuffer* buffer = self;
    if (start >= buffer->length || end <= start) {
        return NULL;
    }
    if (end > buffer->length) {
        end = buffer->length;
    }
    size_t deleteLen = end - start;
    size_t strLen = strlen(str);
    size_t newLength = buffer->length - deleteLen + strLen;
    if (ooc_stringBufferEnsureCapacity(self, newLength + 1) != OOC_ERROR_NONE) {
        return NULL;
    }
    memmove(buffer->data + start + strLen, buffer->data + end, buffer->length - end + 1);
    memcpy(buffer->data + start, str, strLen);
    buffer->length = newLength;
    return self;
}

void* ooc_stringBufferReplaceString(void* self, const void* target, const void* replacement) {
    return ooc_stringBufferReplaceCString(self, ooc_stringGetCString(target), ooc_stringGetCString(replacement));
}

void* ooc_stringBufferReplaceCString(void* self, const char* target, const char* replacement) {
    if (!self || !target || !replacement) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    size_t targetLen = strlen(target);
    if (targetLen == 0) {
        return self;
    }
    int index;
    while ((index = ooc_stringBufferIndexOfCString(self, target)) != -1) {
        ooc_stringBufferReplaceWithCString(self, index, index + targetLen, replacement);
    }
    return self;
}

void* ooc_stringBufferReplaceFirstString(void* self, const void* target, const void* replacement) {
    return ooc_stringBufferReplaceFirstCString(self, ooc_stringGetCString(target), ooc_stringGetCString(replacement));
}

void* ooc_stringBufferReplaceFirstCString(void* self, const char* target, const char* replacement) {
    if (!self || !target || !replacement) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    size_t len = strlen(target);
    int index = ooc_stringBufferIndexOfCString(self, target);
    if (index == -1) {
        return self;
    }
    return ooc_stringBufferReplaceWithCString(self, index, index + len, replacement);
}

void* ooc_stringBufferReplaceLastString(void* self, const void* target, const void* replacement) {
    return ooc_stringBufferReplaceLastCString(self, ooc_stringGetCString(target), ooc_stringGetCString(replacement));
}

void* ooc_stringBufferReplaceLastCString(void* self, const char* target, const char* replacement) {
    if (!self || !target || !replacement) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    size_t len = strlen(target);
    int index = ooc_stringBufferLastIndexOfCString(self, target);
    if (index == -1) {
        return self;
    }
    return ooc_stringBufferReplaceWithCString(self, index, index + len, replacement);
}

void* ooc_stringBufferReplaceChar(void* self, char oldChar, char newChar) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_StringBuffer* buffer = self;
    if (ooc_StringReplaceChar(buffer->data, oldChar, newChar) != OOC_ERROR_NONE) {
        return NULL;
    }
    return self;
}

void* ooc_stringBufferToLowerCase(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_StringBuffer* buffer = self;
    if (ooc_StringToLowerCase(buffer->data) != OOC_ERROR_NONE) {
        return NULL;
    }
    return self;
}

void* ooc_stringBufferToUpperCase(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_StringBuffer* buffer = self;
    if (ooc_StringToUpperCase(buffer->data) != OOC_ERROR_NONE) {
        return NULL;
    }
    return self;
}

void* ooc_stringBufferTrim(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_StringBuffer* buffer = self;
    buffer->data = ooc_StringTrim(buffer->data);
    buffer->length = strlen(buffer->data);
    return self;
}

OOC_Error ooc_stringBufferTrimToSize(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_StringBuffer* buffer = self;
    size_t newCapacity = buffer->length + 1;
    if (newCapacity == buffer->capacity) {
        return OOC_ERROR_NONE;
    }
    char* newData = realloc(buffer->data, newCapacity);
    if (!newData) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    buffer->data = newData;
    buffer->capacity = newCapacity;
    return OOC_ERROR_NONE;
}

void* ooc_stringBufferAppendString(void* self, const void* string) {
    if (!self || !string) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_TYPE_CHECK(string, ooc_stringClass(), NULL);
    return ooc_stringBufferAppendCString(self, ooc_stringGetCString(string));;
}

void* ooc_stringBufferAppendCString(void* self, const char* str) {
    if (!self || !str) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_StringBuffer* buffer = self;
    size_t strLen = strlen(str);
    if (strLen == 0) {
        return self;
    }
    size_t newLength = buffer->length + strLen;
    if (ooc_stringBufferEnsureCapacity(self, newLength + 1) != OOC_ERROR_NONE) {
        return NULL;
    }
    strcpy(buffer->data + buffer->length, str);
    buffer->length = newLength;
    return self;
}

void* ooc_stringBufferAppendChar(void* self, char ch) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    char temp[2];
    if (ooc_StringFromCharAt(temp, sizeof(temp), ch) != OOC_ERROR_NONE) {
        return NULL;
    }
    return ooc_stringBufferAppendCString(self, temp);
}

void* ooc_stringBufferAppendInt(void* self, int value) {
    if (!self) {
        return NULL;
    }
    char temp[32];
    if (ooc_StringFromIntAt(temp, sizeof(temp), value) != OOC_ERROR_NONE) {
        return NULL;
    }
    return ooc_stringBufferAppendCString(self, temp);
}

void* ooc_stringBufferAppendDouble(void* self, double value) {
    if (!self) {
        return NULL;
    }
    char temp[64];
    if (ooc_StringFromDoubleAt(temp, sizeof(temp), value) != OOC_ERROR_NONE) {
        return NULL;
    }
    return ooc_stringBufferAppendCString(self, temp);
}

void* ooc_stringBufferAppendBool(void* self, bool value) {
    if (!self) {
        return NULL;
    }
    char temp[10];
    if (ooc_StringFromBoolAt(temp, sizeof(temp), value) != OOC_ERROR_NONE) {
        return NULL;
    }
    return ooc_stringBufferAppendCString(self, temp);
}

void* ooc_stringBufferAppendObject(void* self, const void* object) {
    if (!self || !object) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_TYPE_CHECK(object, ooc_objectClass(), NULL);
    char* string = ooc_toString(object);
    ooc_stringBufferAppendCString(self, string);
    free(string);
    return self;
}

void* ooc_stringBufferAppendFormat(void* self, const char* format, ...) {
    if (!self || !format) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    va_list args;
    va_start(args, format);
    char* string = ooc_StringFormatv(format, &args);
    va_end(args);
    if (!string) {
        return NULL;
    }
    void* result = ooc_stringBufferAppendCString(self, string);
    free(string);
    return result;
}

void* ooc_stringBufferInsertString(void* self, size_t offset, const void* string) {
    if (!self || !string) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_TYPE_CHECK(string, ooc_stringClass(), NULL);
    return ooc_stringBufferInsertCString(self, offset, ooc_stringGetCString(string));
}

void* ooc_stringBufferInsertCString(void* self, size_t offset, const char* str) {
    if (!self || !str) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_StringBuffer* buffer = self;
    if (offset > buffer->length) {
        return NULL;
    }
    size_t strLen = strlen(str);
    if (strLen == 0) {
        return self;
    }
    size_t newLength = buffer->length + strLen;
    if (ooc_stringBufferEnsureCapacity(self, newLength + 1) != OOC_ERROR_NONE) {
        return NULL;
    }
    memmove(buffer->data + offset + strLen, buffer->data + offset, buffer->length - offset + 1);
    memcpy(buffer->data + offset, str, strLen);
    buffer->length = newLength;
    return self;
}

void* ooc_stringBufferInsertChar(void* self, size_t offset, char ch) {
    char temp[2];
    if (ooc_StringFromCharAt(temp, sizeof(temp), ch) != OOC_ERROR_NONE) {
        return NULL;
    }
    return ooc_stringBufferInsertCString(self, offset, temp);
}

void* ooc_stringBufferInsertInt(void* self, size_t offset, int value) {
    char temp[32];
    if (ooc_StringFromIntAt(temp, sizeof(temp), value) != OOC_ERROR_NONE) {
        return NULL;
    }
    return ooc_stringBufferInsertCString(self, offset, temp);
}

void* ooc_stringBufferInsertDouble(void* self, size_t offset, double value) {
    char temp[64];
    if (ooc_StringFromDoubleAt(temp, sizeof(temp), value) != OOC_ERROR_NONE) {
        return NULL;
    }
    return ooc_stringBufferInsertCString(self, offset, temp);
}

void* ooc_stringBufferInsertBool(void* self, size_t offset, bool value) {
    char temp[10];
    if (ooc_StringFromBoolAt(temp, sizeof(temp), value) != OOC_ERROR_NONE) {
        return NULL;
    }
    return ooc_stringBufferInsertCString(self, offset, temp);
}

void* ooc_stringBufferInsertObject(void* self, size_t offset, const void* object) {
    if (!self || !object) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_TYPE_CHECK(object, ooc_objectClass(), NULL);
    char* string = ooc_toString(object);
    ooc_stringBufferInsertCString(self, offset, string);
    free(string);
    return self;
}

void* ooc_stringBufferInsertFormat(void* self, size_t offset, const char* format, ...) {
    if (!self || !format) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    va_list args;
    va_start(args, format);
    char* string = ooc_StringFormatv(format, &args);
    va_end(args);
    if (!string) {
        return NULL;
    }
    void* result = ooc_stringBufferInsertCString(self, offset, string);
    free(string);
    return result;
}

void* ooc_stringBufferDelete(void* self, size_t start, size_t end) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_StringBuffer* buffer = self;
    if (start >= buffer->length || end <= start) {
        return NULL;
    }
    if (end > buffer->length) {
        end = buffer->length;
    }
    size_t deleteLen = end - start;
    memmove(buffer->data + start, buffer->data + end, buffer->length - end + 1);
    buffer->length -= deleteLen;
    return self;
}

void* ooc_stringBufferDeleteCharAt(void* self, size_t index) {
    return ooc_stringBufferDelete(self, index, index + 1);
}

void* ooc_stringBufferReverse(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    OOC_StringBuffer* buffer = self;
    if (ooc_StringReverse(buffer->data) != OOC_ERROR_NONE) {
        return NULL;
    }
    return self;
}

OOC_Error ooc_stringBufferClear(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_StringBuffer* buffer = self;
    buffer->length = 0;
    buffer->data[0] = '\0';
    return OOC_ERROR_NONE;
}

int ooc_stringBufferIndexOfChar(const void* self, char ch) {
    if (!self) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), -1);
    const OOC_StringBuffer* buffer = self;
    return ooc_StringIndexOfChar(buffer->data, ch);
}

int ooc_stringBufferLastIndexOfChar(const void* self, char ch) {
    if (!self) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), -1);
    const OOC_StringBuffer* buffer = self;
    return ooc_StringLastIndexOfChar(buffer->data, ch);
}

int ooc_stringBufferIndexOfString(const void* self, const void* str) {
    return ooc_stringBufferIndexOfCString(self, ooc_stringGetCString(str));
}

int ooc_stringBufferIndexOfCString(const void* self, const char* str) {
    if (!self || !str) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), -1);
    const OOC_StringBuffer* buffer = self;
    return ooc_StringIndexOfString(buffer->data, str);
}

int ooc_stringBufferLastIndexOfString(const void* self, const void* str) {
    return ooc_stringBufferLastIndexOfCString(self, ooc_stringGetCString(str));
}

int ooc_stringBufferLastIndexOfCString(const void* self, const char* str) {
    if (!self || !str) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), -1);
    const OOC_StringBuffer* buffer = self;
    return ooc_StringLastIndexOfString(buffer->data, str);
}

bool ooc_stringBufferStartsWithString(const void* self, const void* prefix) {
    return ooc_stringBufferStartsWithCString(self, ooc_stringGetCString(prefix));
}

bool ooc_stringBufferStartsWithCString(const void* self, const char* prefix) {
    if (!self || !prefix) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), false);
    const OOC_StringBuffer* buffer = self;
    return ooc_StringStartsWithString(buffer->data, prefix);
}

bool ooc_stringBufferEndsWithString(const void* self, const void* suffix) {
    return ooc_stringBufferEndsWithCString(self, ooc_stringGetCString(suffix));
}

bool ooc_stringBufferEndsWithCString(const void* self, const char* suffix) {
    if (!self || !suffix) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), false);
    const OOC_StringBuffer* buffer = self;
    return ooc_StringEndsWithString(buffer->data, suffix);
}

bool ooc_stringBufferContainsString(const void* self, const void* str) {
    return ooc_stringBufferContainsCString(self, ooc_stringGetCString(str));
}

bool ooc_stringBufferContainsCString(const void* self, const char* str) {
    if (!self || !str) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), false);
    const OOC_StringBuffer* buffer = self;
    return ooc_StringContainsString(buffer->data, str);
}

char* ooc_stringBufferToCString(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    const OOC_StringBuffer* buffer = self;
    char* result = malloc(buffer->length + 1);
    if (!result) {
        return NULL;
    }
    strcpy(result, buffer->data);
    return result;
}

const char* ooc_stringBufferGetCString(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    const OOC_StringBuffer* buffer = self;
    return buffer->data;
}

void* ooc_stringBufferToString(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringBufferClass(), NULL);
    const OOC_StringBuffer* buffer = self;
    return ooc_new(ooc_stringClass(), buffer->data);
}
