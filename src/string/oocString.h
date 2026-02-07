/**
 * @file oocString.h
 * @brief Object-oriented C String class implementation
 *
 * This header defines a comprehensive String class for the OOC framework,
 * providing both static utility functions for C strings and instance methods
 * for String objects. The implementation follows object-oriented principles
 * adapted for C.
 *
 * String objects are immutable. All instance methods that appear to modify
 * a String object actually return a new String object with the modifications,
 * leaving the original String object unchanged. This ensures thread-safety
 * and prevents unintended side effects.
 */

#ifndef OOC_STRING_H_
#define OOC_STRING_H_

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

#include "oocError.h"

/**
 * @brief Returns the String class descriptor
 * @return Pointer to the String class object
 */
void* ooc_stringClass();

// ============================================================================
// Static methods for C strings (equivalent to Java static methods)
// ============================================================================

/**
 * @brief Creates a formatted string using a va_list
 * @param format Printf-style format string
 * @param args Pointer to an initialized va_list
 * @return Newly allocated formatted string, or NULL on failure
 * @note Caller is responsible for freeing the returned string
 */
char* ooc_StringFormatv(const char* format, va_list* args);

/**
 * @brief Creates a formatted string using printf-style formatting
 * @param format Printf-style format string
 * @param ... Variable arguments for the format string
 * @return Newly allocated formatted string, or NULL on failure
 * @note Caller is responsible for freeing the returned string
 */
char* ooc_StringFormat(const char* format, ...);

/**
 * @brief Converts an integer to its string representation
 * @param value Integer value to convert
 * @return Newly allocated string representation, or NULL on failure
 * @note Caller is responsible for freeing the returned string
 */
char* ooc_StringFromInt(int value);

/**
 * @brief Converts a double to its string representation
 * @param value Double value to convert
 * @return Newly allocated string representation, or NULL on failure
 * @note Caller is responsible for freeing the returned string
 */
char* ooc_StringFromDouble(double value);

/**
 * @brief Converts a character to its string representation
 * @param value Character value to convert
 * @return Newly allocated string representation, or NULL on failure
 * @note Caller is responsible for freeing the returned string
 */
char* ooc_StringFromChar(char value);

/**
 * @brief Converts a boolean to its string representation
 * @param value Boolean value to convert
 * @return Newly allocated string representation ("true" or "false"), or NULL on failure
 * @note Caller is responsible for freeing the returned string
 */
char* ooc_StringFromBool(bool value);

/**
 * @brief Creates a formatted string in a provided buffer
 * @param buffer Destination buffer for the formatted string
 * @param bufferSize Size of the destination buffer
 * @param format Printf-style format string
 * @param ... Variable arguments for the format string
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_StringFormatAt(char* buffer, size_t bufferSize, const char* format, ...);

/**
 * @brief Converts an integer to string in a provided buffer
 * @param buffer Destination buffer for the string
 * @param bufferSize Size of the destination buffer
 * @param value Integer value to convert
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_StringFromIntAt(char* buffer, size_t bufferSize, int value);

/**
 * @brief Converts a double to string in a provided buffer
 * @param buffer Destination buffer for the string
 * @param bufferSize Size of the destination buffer
 * @param value Double value to convert
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_StringFromDoubleAt(char* buffer, size_t bufferSize, double value);

/**
 * @brief Converts a character to string in a provided buffer
 * @param buffer Destination buffer for the string
 * @param bufferSize Size of the destination buffer
 * @param value Character value to convert
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_StringFromCharAt(char* buffer, size_t bufferSize, char value);

/**
 * @brief Converts a boolean to string in a provided buffer
 * @param buffer Destination buffer for the string
 * @param bufferSize Size of the destination buffer
 * @param value Boolean value to convert
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_StringFromBoolAt(char* buffer, size_t bufferSize, bool value);

// ============================================================================
// Static versions of instance methods - operate on C strings
// ============================================================================

/**
 * @brief Returns the length of a C string
 * @param str Null-terminated C string
 * @return Length of the string, or 0 if str is NULL
 */
size_t ooc_StringLength(const char* str);

/**
 * @brief Returns the character at the specified index
 * @param str Null-terminated C string
 * @param index Zero-based index of the character
 * @return Character at the specified index.
 *         Returns '\0' if index is out of bounds or str is NULL.
 */
char ooc_StringCharAt(const char* str, size_t index);

/**
 * @brief Sets a character at the specified index in a C string
 * @param str Null-terminated C string to modify
 * @param index Zero-based index
 * @param ch Character to set
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note The string must be mutable and large enough
 */
OOC_Error ooc_StringSetCharAt(char* str, size_t index, char ch);

/**
 * @brief Extracts a substring from the given string
 * @param str Source null-terminated C string
 * @param start Starting index (inclusive)
 * @param end Ending index (exclusive)
 * @return Newly allocated substring, or NULL on failure
 * @note Caller is responsible for freeing the returned string
 */
char* ooc_StringSubstring(const char* str, size_t start, size_t end);

/**
 * @brief Extracts a substring into a provided buffer
 * @param buffer Destination buffer for the substring
 * @param bufferSize Size of the destination buffer
 * @param str Source null-terminated C string
 * @param start Starting index (inclusive)
 * @param end Ending index (exclusive)
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_StringSubstringAt(char* buffer, size_t bufferSize, const char* str, size_t start, size_t end);

/**
 * @brief Concatenates two C strings
 * @param str1 First null-terminated C string
 * @param str2 Second null-terminated C string
 * @return Newly allocated concatenated string, or NULL on failure
 * @note Caller is responsible for freeing the returned string
 */
char* ooc_StringConcat(const char* str1, const char* str2);

/**
 * @brief Concatenates two C strings into a provided buffer
 * @param buffer Destination buffer for the concatenated string
 * @param bufferSize Size of the destination buffer
 * @param str1 First null-terminated C string
 * @param str2 Second null-terminated C string
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_StringConcatAt(char* buffer, size_t bufferSize, const char* str1, const char* str2);

/**
 * @brief Checks if a string starts with the specified prefix
 * @param str Null-terminated C string to check
 * @param prefix Null-terminated prefix string
 * @return true if str starts with prefix, false otherwise
 */
bool ooc_StringStartsWithString(const char* str, const char* prefix);

/**
 * @brief Checks if a string ends with the specified suffix
 * @param str Null-terminated C string to check
 * @param suffix Null-terminated suffix string
 * @return true if str ends with suffix, false otherwise
 */
bool ooc_StringEndsWithString(const char* str, const char* suffix);

/**
 * @brief Finds the first occurrence of a character in the string
 * @param str Null-terminated C string to search in
 * @param ch Character to search for
 * @return Index of the first occurrence, or -1 if not found
 */
int ooc_StringIndexOfChar(const char* str, char ch);

/**
 * @brief Finds the last occurrence of a character in the string
 * @param str Null-terminated C string to search in
 * @param ch Character to search for
 * @return Index of the last occurrence, or -1 if not found
 */
int ooc_StringLastIndexOfChar(const char* str, char ch);

/**
 * @brief Finds the first occurrence of a substring in a string
 * @param str Null-terminated C string to search in
 * @param sub Null-terminated substring to search for
 * @return Zero-based index of the first occurrence, or -1 if not found
 */
int ooc_StringIndexOfString(const char* str, const char* sub);

/**
 * @brief Finds the last occurrence of a substring in a string
 * @param str Null-terminated C string to search in
 * @param sub Null-terminated substring to search for
 * @return Zero-based index of the last occurrence, or -1 if not found
 */
int ooc_StringLastIndexOfString(const char* str, const char* sub);

/**
 * @brief Replaces all occurrences of a character with another character
 * @param str Null-terminated C string to modify (modified in place)
 * @param oldChar Character to replace
 * @param newChar Replacement character
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_StringReplaceChar(char* str, char oldChar, char newChar);

/**
 * @brief Converts all characters in the string to lowercase
 * @param str Null-terminated C string to modify (modified in place)
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_StringToLowerCase(char* str);

/**
 * @brief Converts all characters in the string to uppercase
 * @param str Null-terminated C string to modify (modified in place)
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_StringToUpperCase(char* str);

/**
 * @brief Removes leading and trailing whitespace from the string
 * @param str Null-terminated C string to trim (modified in place)
 * @return Pointer to the trimmed string (same as input), or NULL if str is NULL
 */
char* ooc_StringTrim(char* str);

/**
 * @brief Checks if a string is empty (NULL or zero length)
 * @param str Null-terminated C string to check
 * @return true if the string is NULL or empty, false otherwise
 */
bool ooc_StringIsEmpty(const char* str);

/**
 * @brief Reverses a C string in place
 * @param str Null-terminated C string to reverse
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_StringReverse(char* str);

/**
 * @brief Checks if a string contains a substring
 * @param str Null-terminated C string to search in
 * @param sub Null-terminated substring to search for
 * @return true if the substring is found, false otherwise
 */
bool ooc_StringContainsString(const char* str, const char* sub);

// ============================================================================
// Instance methods - operate on String objects
// ============================================================================

/**
 * @brief Returns the length of a String object
 * @param self String object instance
 * @return Length of the string, or 0 if self is NULL
 */
size_t ooc_stringLength(const void* self);

/**
 * @brief Returns the character at the specified index in a String object
 * @param self String object instance
 * @param index Zero-based index of the character
 * @return Character at the specified index.
 *         Returns '\0' if index is out of bounds or self is NULL.
 */
char ooc_stringCharAt(const void* self, size_t index);

/**
 * @brief Extracts a substring from a String object
 * @param self String object instance
 * @param start Starting index (inclusive)
 * @param end Ending index (exclusive)
 * @return New String object containing the substring, or NULL on failure
 * @note Caller is responsible for destroying the returned String object with ooc_destroy
 */
void* ooc_stringSubstring(const void* self, size_t start, size_t end);

/**
 * @brief Concatenates two String objects
 * @param self First String object instance
 * @param other Second String object instance
 * @return New String object containing the concatenated result, or NULL on failure
 * @note Caller is responsible for destroying the returned String object with ooc_destroy
 */
void* ooc_stringConcat(const void* self, const void* other);

/**
 * @brief Checks if a String object starts with another String object
 * @param self String object instance to check
 * @param prefix String object instance containing the prefix
 * @return true if self starts with prefix, false otherwise
 */
bool ooc_stringStartsWithString(const void* self, const void* prefix);

/**
 * @brief Checks if a String object starts with a C string
 * @param self String object instance to check
 * @param prefix Null-terminated C string containing the prefix
 * @return true if self starts with prefix, false otherwise
 */
bool ooc_stringStartsWithCString(const void* self, const char* prefix);

/**
 * @brief Checks if a String object ends with another String object
 * @param self String object instance to check
 * @param suffix String object instance containing the suffix
 * @return true if self ends with suffix, false otherwise
 */
bool ooc_stringEndsWithString(const void* self, const void* suffix);

/**
 * @brief Checks if a String object ends with a C string
 * @param self String object instance to check
 * @param suffix Null-terminated C string containing the suffix
 * @return true if self ends with suffix, false otherwise
 */
bool ooc_stringEndsWithCString(const void* self, const char* suffix);

/**
 * @brief Finds the first occurrence of a character in a String object
 * @param self String object instance to search in
 * @param ch Character to search for
 * @return Index of the first occurrence, or -1 if not found
 */
int ooc_stringIndexOfChar(const void* self, char ch);

/**
 * @brief Finds the last occurrence of a character in a String object
 * @param self String object instance to search in
 * @param ch Character to search for
 * @return Index of the last occurrence, or -1 if not found
 */
int ooc_stringLastIndexOfChar(const void* self, char ch);

/**
 * @brief Finds the first occurrence of a substring in a String object
 * @param self String object instance to search in
 * @param sub String object instance containing the substring to search for
 * @return Zero-based index of the first occurrence, or -1 if not found
 */
int ooc_stringIndexOfString(const void* self, const void* sub);

/**
 * @brief Finds the first occurrence of a C string in a String object
 * @param self String object instance to search in
 * @param sub Null-terminated C string containing the substring to search for
 * @return Zero-based index of the first occurrence, or -1 if not found
 */
int ooc_stringIndexOfCString(const void* self, const char* sub);

/**
 * @brief Finds the last occurrence of a substring in a String object
 * @param self String object instance to search in
 * @param sub String object instance containing the substring to search for
 * @return Zero-based index of the last occurrence, or -1 if not found
 */
int ooc_stringLastIndexOfString(const void* self, const void* sub);

/**
 * @brief Finds the last occurrence of a C string in a String object
 * @param self String object instance to search in
 * @param sub Null-terminated C string containing the substring to search for
 * @return Zero-based index of the last occurrence, or -1 if not found
 */
int ooc_stringLastIndexOfCString(const void* self, const char* sub);

/**
 * @brief Replaces all occurrences of a character in a String object
 * @param self String object instance
 * @param oldChar Character to replace
 * @param newChar Replacement character
 * @return New String object with characters replaced, or NULL on failure
 * @note Caller is responsible for destroying the returned String object with ooc_destroy
 */
void* ooc_stringReplaceChar(const void* self, char oldChar, char newChar);

/**
 * @brief Converts all characters in a String object to lowercase
 * @param self String object instance
 * @return New String object with lowercase characters, or NULL on failure
 * @note Caller is responsible for destroying the returned String object with ooc_destroy
 */
void* ooc_stringToLowerCase(const void* self);

/**
 * @brief Converts all characters in a String object to uppercase
 * @param self String object instance
 * @return New String object with uppercase characters, or NULL on failure
 * @note Caller is responsible for destroying the returned String object with ooc_destroy
 */
void* ooc_stringToUpperCase(const void* self);

/**
 * @brief Removes leading and trailing whitespace from a String object
 * @param self String object instance
 * @return New String object with whitespace removed, or NULL on failure
 * @note Caller is responsible for destroying the returned String object with ooc_destroy
 */
void* ooc_stringTrim(const void* self);

/**
 * @brief Checks if a String object is empty
 * @param self String object instance to check
 * @return true if the string is NULL or empty, false otherwise
 */
bool ooc_stringIsEmpty(const void* self);

/**
 * @brief Checks if a String object contains another String object
 * @param self String object instance to search in
 * @param sub String object instance containing the substring to search for
 * @return true if the substring is found, false otherwise
 */
bool ooc_stringContainsString(const void* self, const void* sub);

/**
 * @brief Checks if a String object contains a C string
 * @param self String object instance to search in
 * @param sub Null-terminated C string containing the substring to search for
 * @return true if the substring is found, false otherwise
 */
bool ooc_stringContainsCString(const void* self, const char* sub);

/**
 * @brief Returns the C string representation of a String object
 * @param self String object instance
 * @return Newly allocated C string, or NULL if self is NULL
 * @note Caller is responsible for freeing the returned string
 */
char* ooc_stringToCString(const void* self);

/**
 * @brief Copies the string content to a provided buffer
 * @param buffer Destination buffer for the string content
 * @param bufferSize Size of the destination buffer
 * @param self String object instance
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_stringToCStringAt(const void* self, char* buffer, size_t bufferSize);

/**
 * @brief Returns a const pointer to the internal C string (read-only access)
 * @param self String object instance
 * @return Const pointer to the internal C string, or NULL if self is NULL
 * @note The returned pointer must not be modified or freed.
 *       The pointer remains valid for the lifetime of the String object.
 */
const char* ooc_stringGetCString(const void* self);

#endif /* OOC_STRING_H_ */
