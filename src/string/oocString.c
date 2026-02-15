#include "oocString.h"
#include "oocString.r"
#include "oocObject.h"
#include "oocStringBuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

static OOC_StringClass* StringClass;
static OOC_StringClass StringClassInstance;

static OOC_Error ooc_stringConstructor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_Error error = ooc_superCtor(self, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    OOC_String* string = self;
    const char* str = va_arg(*args, const char*);
    string->stringBuffer = ooc_new(ooc_stringBufferClass(), str);
    if (!string->stringBuffer) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_stringDestructor(void* self) {
    if (!self) {
        return OOC_ERROR_NONE;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_String* string = self;
    OOC_Error error = ooc_destroy(string->stringBuffer);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    return ooc_superDtor(self);
}

static char* ooc_stringToString(const void* self) {
    return ooc_stringToCString(self);
}

static bool ooc_stringEquals(const void* self, const void* other) {
    if (!self || !other) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), false);
    OOC_TYPE_CHECK(other, ooc_stringClass(), false);
    const OOC_String* str1 = self;
    const OOC_String* str2 = other;
    return ooc_equals(str1->stringBuffer, str2->stringBuffer);
}

static size_t ooc_stringHash(const void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), 0);
    const OOC_String* string = self;
    return ooc_hashCode(string->stringBuffer);
}

static int ooc_stringCompare(const void* self, const void* other) {
    if (!self || !other) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), -1);
    OOC_TYPE_CHECK(other, ooc_stringClass(), -1);
    const OOC_String* str1 = self;
    const OOC_String* str2 = other;
    return ooc_compare(str1->stringBuffer, str2->stringBuffer);
}

static void* ooc_stringClone(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), NULL);
    return ooc_new(ooc_stringClass(), ooc_stringGetCString(self));
}

static void* ooc_stringClassInit(void) {
    if (ooc_classNew(&StringClassInstance,
                    "String",
                    sizeof(OOC_String),
                    sizeof(OOC_StringClass),
                    ooc_objectClass(),
                    OOC_MODIFIER_FINAL,
                    OOC_METHOD_CTOR, ooc_stringConstructor,
                    OOC_METHOD_DTOR, ooc_stringDestructor,
                    OOC_METHOD_TO_STRING, ooc_stringToString,
                    OOC_METHOD_EQUALS, ooc_stringEquals,
                    OOC_METHOD_HASH, ooc_stringHash,
                    OOC_METHOD_COMPARE, ooc_stringCompare,
                    OOC_METHOD_CLONE, ooc_stringClone,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &StringClassInstance;
}

void* ooc_stringClass() {
    if (!StringClass) {
        StringClass = ooc_stringClassInit();
    }
    return StringClass;
}

char* ooc_StringFormatv(const char* format, va_list* args) {
    if (!format || !args) {
        return NULL;
    }
    va_list argsCopy;
    va_copy(argsCopy, *args);
    int size = vsnprintf(NULL, 0, format, argsCopy);
    va_end(argsCopy);
    if (size < 0) {
        return NULL;
    }
    char* result = malloc((size_t)size + 1);
    if (!result) {
        return NULL;
    }
    vsnprintf(result, (size_t)size + 1, format, *args);
    return result;
}

char* ooc_StringFormat(const char* format, ...) {
    if (!format) {
        return NULL;
    }
    va_list args;
    va_start(args, format);
    char* result = ooc_StringFormatv(format, &args);
    va_end(args);
    return result;
}

char* ooc_StringFromInt(int value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", value);
    char* result = malloc(strlen(buffer) + 1);
    if (result) {
        strcpy(result, buffer);
    }
    return result;
}

char* ooc_StringFromDouble(double value) {
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%g", value);
    char* result = malloc(strlen(buffer) + 1);
    if (result) {
        strcpy(result, buffer);
    }
    return result;
}

char* ooc_StringFromChar(char value) {
    char* result = malloc(2);
    if (result) {
        result[0] = value;
        result[1] = '\0';
    }
    return result;
}

char* ooc_StringFromBool(bool value) {
    const char* str = value ? "true" : "false";
    char* result = malloc(strlen(str) + 1);
    if (result) {
        strcpy(result, str);
    }
    return result;
}

OOC_Error ooc_StringFormatAt(char* buffer, size_t bufferSize, const char* format, ...) {
    if (!buffer || bufferSize == 0 || !format) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    va_list args;
    va_start(args, format);
    int result = vsnprintf(buffer, bufferSize, format, args);
    va_end(args);
    if (result < 0) {
        return OOC_ERROR_STANDARD;
    }
    if (result >= (int)bufferSize) {
        return OOC_ERROR_BUFFER_TOO_SMALL;
    }
    return OOC_ERROR_NONE;
}

OOC_Error ooc_StringFromIntAt(char* buffer, size_t bufferSize, int value) {
    if (!buffer || bufferSize == 0) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    int result = snprintf(buffer, bufferSize, "%d", value);
    if (result < 0) {
        return OOC_ERROR_STANDARD;
    }
    if (result >= (int)bufferSize) {
        return OOC_ERROR_BUFFER_TOO_SMALL;
    }
    return OOC_ERROR_NONE;
}

OOC_Error ooc_StringFromDoubleAt(char* buffer, size_t bufferSize, double value) {
    if (!buffer || bufferSize == 0) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    int result = snprintf(buffer, bufferSize, "%g", value);
    if (result < 0) {
        return OOC_ERROR_STANDARD;
    }
    if (result >= (int)bufferSize) {
        return OOC_ERROR_BUFFER_TOO_SMALL;
    }
    return OOC_ERROR_NONE;
}

OOC_Error ooc_StringFromCharAt(char* buffer, size_t bufferSize, char value) {
    if (!buffer || bufferSize < 2) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    buffer[0] = value;
    buffer[1] = '\0';
    return OOC_ERROR_NONE;
}

OOC_Error ooc_StringFromBoolAt(char* buffer, size_t bufferSize, bool value) {
    if (!buffer || bufferSize == 0) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const char* str = value ? "true" : "false";
    size_t len = strlen(str);
    if (len >= bufferSize) {
        return OOC_ERROR_BUFFER_TOO_SMALL;
    }
    strcpy(buffer, str);
    return OOC_ERROR_NONE;
}

size_t ooc_StringLength(const char* str) {
    return str ? strlen(str) : 0;
}

char ooc_StringCharAt(const char* str, size_t index) {
    if (!str) {
        return '\0';
    }
    size_t len = strlen(str);
    if (index >= len) {
        return '\0';
    }
    return str[index];
}

OOC_Error ooc_StringSetCharAt(char* str, size_t index, char ch) {
    if (!str) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    size_t len = strlen(str);
    if (index >= len) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    str[index] = ch;
    return OOC_ERROR_NONE;
}

char* ooc_StringSubstring(const char* str, size_t start, size_t end) {
    if (!str) {
        return NULL;
    }
    size_t len = strlen(str);
    if (start >= len || end <= start) {
        return NULL;
    }
    if (end > len) {
        end = len;
    }
    size_t subLen = end - start;
    char* result = malloc(subLen + 1);
    if (!result) {
        return NULL;
    }
    strncpy(result, str + start, subLen);
    result[subLen] = '\0';
    return result;
}

OOC_Error ooc_StringSubstringAt(char* buffer, size_t bufferSize, const char* str, size_t start, size_t end) {
    if (!buffer || !str || bufferSize == 0) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    size_t len = strlen(str);
    if (start >= len || end <= start) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    if (end > len) {
        end = len;
    }
    size_t subLen = end - start;
    if (subLen >= bufferSize) {
        return OOC_ERROR_BUFFER_TOO_SMALL;
    }
    strncpy(buffer, str + start, subLen);
    buffer[subLen] = '\0';
    return OOC_ERROR_NONE;
}

char* ooc_StringConcat(const char* str1, const char* str2) {
    if (!str1 || !str2) {
        return NULL;
    }
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = malloc(len1 + len2 + 1);
    if (!result) {
        return NULL;
    }
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

OOC_Error ooc_StringConcatAt(char* buffer, size_t bufferSize, const char* str1, const char* str2) {
    if (!buffer || bufferSize == 0 || !str1 || !str2) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    if (len1 + len2 >= bufferSize) {
        return OOC_ERROR_BUFFER_TOO_SMALL;
    }
    strcpy(buffer, str1);
    strcat(buffer, str2);
    return OOC_ERROR_NONE;
}

bool ooc_StringStartsWithString(const char* str, const char* prefix) {
    if (!str || !prefix) {
        return false;
    }
    size_t prefixLen = strlen(prefix);
    if (prefixLen > strlen(str)) {
        return false;
    }
    return strncmp(str, prefix, prefixLen) == 0;
}

bool ooc_StringEndsWithString(const char* str, const char* suffix) {
    if (!str || !suffix) {
        return false;
    }
    size_t strLen = strlen(str);
    size_t suffixLen = strlen(suffix);
    if (suffixLen > strLen) {
        return false;
    }
    return strcmp(str + (strLen - suffixLen), suffix) == 0;
}

int ooc_StringIndexOfChar(const char* str, char ch) {
    if (!str) {
        return -1;
    }
    char* pos = strchr(str, ch);
    return pos ? (int)(pos - str) : -1;
}

int ooc_StringLastIndexOfChar(const char* str, char ch) {
    if (!str) {
        return -1;
    }
    char* pos = strrchr(str, ch);
    return pos ? (int)(pos - str) : -1;
}

int ooc_StringIndexOfString(const char* str, const char* sub) {
    if (!str || !sub) {
        return -1;
    }
    if (*sub == '\0') {
        return 0;
    }
    char* pos = strstr(str, sub);
    return pos ? (int)(pos - str) : -1;
}

int ooc_StringLastIndexOfString(const char* str, const char* sub) {
    if (!str || !sub) {
        return -1;
    }
    size_t strLen = strlen(str);
    size_t subLen = strlen(sub);
    if (subLen == 0) {
        return (int)strLen;
    }
    if (subLen > strLen) {
        return -1;
    }
    for (size_t i = strLen - subLen + 1; i-- > 0;) {
        if (strncmp(str + i, sub, subLen) == 0) {
            return (int)i;
        }
    }
    return -1;
}

OOC_Error ooc_StringReplaceChar(char* str, char oldChar, char newChar) {
    if (!str) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    while (*str) {
        if (*str == oldChar) {
         *str = newChar;
        }
        str++;
    }
    return OOC_ERROR_NONE;
}

OOC_Error ooc_StringToLowerCase(char* str) {
    if (!str) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    while (*str) {
        *str = tolower(*str);
        str++;
    }
    return OOC_ERROR_NONE;
}

OOC_Error ooc_StringToUpperCase(char* str) {
    if (!str) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    while (*str) {
        *str = toupper(*str);
        str++;
    }
    return OOC_ERROR_NONE;
}

char* ooc_StringTrim(char* str) {
    if (!str) {
        return NULL;
    }

    // Find start of non-whitespace
    char* start = str;
    while (*start && isspace(*start)) {
        start++;
    }

    // If string is all whitespace
    if (*start == '\0') {
        str[0] = '\0';
        return str;
    }

    // Find end of non-whitespace
    char* end = str + strlen(str) - 1;
    while (end > start && isspace(*end)) {
        end--;
    }

    // Move trimmed content to beginning of string if needed
    if (start != str) {
        size_t len = end - start + 1;
        memmove(str, start, len);
        str[len] = '\0';
    } else {
        // Just null-terminate at the end
        *(end + 1) = '\0';
    }

    return str;
}

bool ooc_StringIsEmpty(const char* str) {
    return !str || strlen(str) == 0;
}

OOC_Error ooc_StringReverse(char* str) {
    if (!str) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    size_t length = strlen(str);
    if (length <= 1) {
        return OOC_ERROR_NONE;
    }
    size_t left = 0;
    size_t right = length - 1;
    while (left < right) {
        char temp = str[left];
        str[left] = str[right];
        str[right] = temp;
        left++;
        right--;
    }
    return OOC_ERROR_NONE;
}

bool ooc_StringContainsString(const char* str, const char* sub) {
    if (!str || !sub) {
        return false;
    }
    return strstr(str, sub) != NULL;
}

size_t ooc_stringLength(const void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), 0);
    const OOC_String* string = self;
    return ooc_stringBufferLength(string->stringBuffer);
}

char ooc_stringCharAt(const void* self, size_t index) {
    if (!self) {
        return '\0';
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), '\0');
    const OOC_String* string = self;
    return ooc_stringBufferCharAt(string->stringBuffer, index);
}

void *ooc_stringSubstring(const void *self, size_t start, size_t end) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), NULL);
    const OOC_String* string = self;
    char* str = ooc_stringBufferSubstring(string->stringBuffer, start, end);
    void* newString = ooc_new(ooc_stringClass(), str);
    free(str);
    return newString;
}

void* ooc_stringConcat(const void* self, const void* other) {
    if (!self || !other) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), NULL);
    OOC_TYPE_CHECK(other, ooc_stringClass(), NULL);
    const OOC_String* otherString = other;
    OOC_String* newString = ooc_clone(self);
    if (!newString) {
        return NULL;
    }
    ooc_stringBufferAppendCString(newString->stringBuffer, ooc_stringGetCString(otherString));
    return newString;
}

bool ooc_stringStartsWithString(const void* self, const void* prefix) {
    if (!self || !prefix) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), false);
    OOC_TYPE_CHECK(prefix, ooc_stringClass(), false);
    return ooc_StringStartsWithString(ooc_stringGetCString(self), ooc_stringGetCString(prefix));
}

bool ooc_stringStartsWithCString(const void* self, const char* prefix) {
    if (!self || !prefix) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), false);
    return ooc_StringStartsWithString(ooc_stringGetCString(self), prefix);
}

bool ooc_stringEndsWithString(const void* self, const void* suffix) {
    if (!self || !suffix) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), false);
    OOC_TYPE_CHECK(suffix, ooc_stringClass(), false);
    return ooc_StringEndsWithString(ooc_stringGetCString(self), ooc_stringGetCString(suffix));
}

bool ooc_stringEndsWithCString(const void* self, const char* suffix) {
    if (!self || !suffix) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), false);
    return ooc_StringEndsWithString(ooc_stringGetCString(self), suffix);
}

int ooc_stringIndexOfChar(const void* self, char ch) {
    if (!self) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), -1);
    return ooc_StringIndexOfChar(ooc_stringGetCString(self), ch);
}

int ooc_stringLastIndexOfChar(const void* self, char ch) {
    if (!self) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), -1);
    return ooc_StringLastIndexOfChar(ooc_stringGetCString(self), ch);
}

int ooc_stringIndexOfString(const void* self, const void* sub) {
    if (!self || !sub) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), -1);
    OOC_TYPE_CHECK(sub, ooc_stringClass(), -1);
    return ooc_StringIndexOfString(ooc_stringGetCString(self), ooc_stringGetCString(sub));
}

int ooc_stringIndexOfCString(const void* self, const char* sub) {
    if (!self || !sub) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), -1);
    return ooc_StringIndexOfString(ooc_stringGetCString(self), sub);
}

int ooc_stringLastIndexOfString(const void* self, const void* sub) {
    if (!self || !sub) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), -1);
    OOC_TYPE_CHECK(sub, ooc_stringClass(), -1);
    return ooc_StringLastIndexOfString(ooc_stringGetCString(self), ooc_stringGetCString(sub));
}

int ooc_stringLastIndexOfCString(const void* self, const char* sub) {
    if (!self || !sub) {
        return -1;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), -1);
    return ooc_StringLastIndexOfString(ooc_stringGetCString(self), sub);
}

void* ooc_stringReplaceChar(const void* self, char oldChar, char newChar) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), NULL);
    OOC_String* newString = ooc_clone(self);
    ooc_stringBufferReplaceChar(newString->stringBuffer, oldChar, newChar);
    return newString;
}

void* ooc_stringToLowerCase(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), NULL);
    OOC_String* newString = ooc_clone(self);
    ooc_stringBufferToLowerCase(newString->stringBuffer);
    return newString;
}

void* ooc_stringToUpperCase(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), NULL);
    OOC_String* newString = ooc_clone(self);
    ooc_stringBufferToUpperCase(newString->stringBuffer);
    return newString;
}

void* ooc_stringTrim(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), NULL);
    OOC_String* newString = ooc_clone(self);
    ooc_stringBufferTrim(newString->stringBuffer);
    return newString;
}

bool ooc_stringIsEmpty(const void* self) {
    if (!self) {
        return true;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), true);
    const OOC_String* string = self;
    return ooc_stringBufferIsEmpty(string->stringBuffer);
}

bool ooc_stringContainsString(const void* self, const void* sub) {
    if (!self || !sub) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), false);
    OOC_TYPE_CHECK(sub, ooc_stringClass(), false);
    return ooc_StringContainsString(ooc_stringGetCString(self),ooc_stringGetCString(sub));
}

bool ooc_stringContainsCString(const void* self, const char* sub) {
    if (!self || !sub) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), false);
    return ooc_StringContainsString(ooc_stringGetCString(self), sub);
}

char* ooc_stringToCString(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), NULL);
    const OOC_String* string = self;
    return ooc_stringBufferToCString(string->stringBuffer);
}

OOC_Error ooc_stringToCStringAt(const void* self, char* buffer, size_t bufferSize) {
    if (!buffer || !self || bufferSize == 0) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), OOC_ERROR_INVALID_OBJECT);
    const OOC_String* string = self;
    if (ooc_stringBufferLength(string->stringBuffer) >= bufferSize) {
        return OOC_ERROR_BUFFER_TOO_SMALL;
    }
    strcpy(buffer, ooc_stringBufferGetCString(string->stringBuffer));
    return OOC_ERROR_NONE;
}

const char* ooc_stringGetCString(const void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_stringClass(), NULL);
    const OOC_String* string = self;
    return ooc_stringBufferGetCString(string->stringBuffer);
}