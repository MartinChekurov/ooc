/**
 * @file oocStringBuffer.h
 * @brief Object-oriented C StringBuffer class implementation
 *
 * This header defines a StringBuffer class for the OOC framework, providing
 * a mutable sequence of characters. Unlike String objects which are immutable,
 * StringBuffer objects can be modified in place, making them more efficient
 * for building strings through multiple operations.
 *
 * StringBuffer objects are mutable and automatically grow their capacity as
 * needed. This makes them ideal for string concatenation, building strings
 * in loops, or any scenario where a string is constructed incrementally.
 */

#ifndef OOC_STRING_BUFFER_H_
#define OOC_STRING_BUFFER_H_

#include "oocError.h"
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Returns the StringBuffer class descriptor
 * @return Pointer to the StringBuffer class object
 */
void* ooc_stringBufferClass();

// ============================================================================
// Instance methods - operate on StringBuffer objects
// ============================================================================

/**
 * @brief Returns the length of the StringBuffer
 * @param self StringBuffer object instance
 * @return Length of the string content, or 0 if self is NULL
 */
size_t ooc_stringBufferLength(const void* self);

/**
 * @brief Sets the length of the StringBuffer
 * @param self StringBuffer object instance
 * @param newLength New length to set
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note If newLength is less than current length, the buffer is truncated.
 *       If greater, null characters are appended.
 */
OOC_Error ooc_stringBufferSetLength(void* self, size_t newLength);

/**
 * @brief Checks if the StringBuffer is empty
 * @param self StringBuffer object instance
 * @return true if the StringBuffer is NULL or empty, false otherwise
 */
bool ooc_stringBufferIsEmpty(const void* self);

/**
 * @brief Returns the current capacity of the StringBuffer
 * @param self StringBuffer object instance
 * @return Current capacity, or 0 if self is NULL
 */
size_t ooc_stringBufferCapacity(const void* self);

/**
 * @brief Ensures the StringBuffer has at least the specified capacity
 * @param self StringBuffer object instance
 * @param minimumCapacity Minimum capacity to ensure
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_stringBufferEnsureCapacity(void* self, size_t minimumCapacity);

/**
 * @brief Returns the character at the specified index
 * @param self StringBuffer object instance
 * @param index Zero-based index of the character
 * @return Character at the specified index, or '\0' if index is out of bounds or self is NULL
 */
char ooc_stringBufferCharAt(const void* self, size_t index);

/**
 * @brief Sets the character at the specified index
 * @param self StringBuffer object instance
 * @param index Zero-based index of the character
 * @param ch Character to set
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_stringBufferSetCharAt(void* self, size_t index, char ch);

/**
 * @brief Extracts a substring from the StringBuffer
 * @param self StringBuffer object instance
 * @param start Starting index (inclusive)
 * @param end Ending index (exclusive)
 * @return Newly allocated C string containing the substring, or NULL on failure
 * @note Caller is responsible for freeing the returned string
 */
char* ooc_stringBufferSubstring(const void* self, size_t start, size_t end);

/**
 * @brief Concatenates another StringBuffer to this StringBuffer
 * @param self StringBuffer object instance
 * @param other StringBuffer object to concatenate
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferConcat(void* self, const void* other);

/**
 * @brief Replaces characters in a range with a String object
 * @param self StringBuffer object instance
 * @param start Starting index (inclusive)
 * @param end Ending index (exclusive)
 * @param string String object to insert
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferReplaceWithString(void* self, size_t start, size_t end, const void* string);

/**
 * @brief Replaces characters in a range with a C string
 * @param self StringBuffer object instance
 * @param start Starting index (inclusive)
 * @param end Ending index (exclusive)
 * @param str Null-terminated C string to insert
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferReplaceWithCString(void* self, size_t start, size_t end, const char* str);

/**
 * @brief Replaces all occurrences of a target String object with another String object
 * @param self StringBuffer object instance
 * @param target Substring to search for (String object)
 * @param replacement Replacement string (String object)
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferReplaceString(void* self, const void* target, const void* replacement);

/**
 * @brief Replaces all occurrences of a target C string with another C string
 * @param self StringBuffer object instance
 * @param target Substring to search for (C string)
 * @param replacement Replacement string (C string)
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferReplaceCString(void* self, const char* target, const char* replacement);

/**
 * @brief Replaces the first occurrence of a target String object
 * @param self StringBuffer object instance
 * @param target Substring to search for (String object)
 * @param replacement Replacement string (String object)
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferReplaceFirstString(void* self, const void* target, const void* replacement);

/**
 * @brief Replaces the first occurrence of a target C string
 * @param self StringBuffer object instance
 * @param target Substring to search for (C string)
 * @param replacement Replacement string (C string)
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferReplaceFirstCString(void* self, const char* target, const char* replacement);

/**
 * @brief Replaces the last occurrence of a target String object
 * @param self StringBuffer object instance
 * @param target Substring to search for (String object)
 * @param replacement Replacement string (String object)
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferReplaceLastString(void* self, const void* target, const void* replacement);

/**
 * @brief Replaces the last occurrence of a target C string
 * @param self StringBuffer object instance
 * @param target Substring to search for (C string)
 * @param replacement Replacement string (C string)
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferReplaceLastCString(void* self, const char* target, const char* replacement);

/**
 * @brief Replaces all occurrences of a character with another character
 * @param self StringBuffer object instance
 * @param oldChar Character to replace
 * @param newChar Character to replace with
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferReplaceChar(void* self, char oldChar, char newChar);

/**
 * @brief Converts all characters in the StringBuffer to lowercase
 * @param self StringBuffer object instance
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferToLowerCase(void* self);

/**
 * @brief Converts all characters in the StringBuffer to uppercase
 * @param self StringBuffer object instance
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferToUpperCase(void* self);

/**
 * @brief Trims whitespace from both ends of the StringBuffer
 * @param self StringBuffer object instance
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferTrim(void* self);

/**
 * @brief Reduces the capacity to match the current length
 * @param self StringBuffer object instance
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_stringBufferTrimToSize(void* self);

/**
 * @brief Appends a String object to the StringBuffer
 * @param self StringBuffer object instance
 * @param string String object to append
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferAppendString(void* self, const void* string);

/**
 * @brief Appends a C string to the StringBuffer
 * @param self StringBuffer object instance
 * @param str Null-terminated C string to append
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferAppendCString(void* self, const char* str);

/**
 * @brief Appends a character to the StringBuffer
 * @param self StringBuffer object instance
 * @param ch Character to append
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferAppendChar(void* self, char ch);

/**
 * @brief Appends an integer to the StringBuffer
 * @param self StringBuffer object instance
 * @param value Integer value to append
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferAppendInt(void* self, int value);

/**
 * @brief Appends a double to the StringBuffer
 * @param self StringBuffer object instance
 * @param value Double value to append
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferAppendDouble(void* self, double value);

/**
 * @brief Appends a boolean to the StringBuffer
 * @param self StringBuffer object instance
 * @param value Boolean value to append (will append "true" or "false")
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferAppendBool(void* self, bool value);

/**
 * @brief Appends any object to the StringBuffer by calling its toString method
 * @param self StringBuffer object instance
 * @param object Object to append (must have a toString method)
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferAppendObject(void* self, const void* object);

/**
 * @brief Appends formatted text (printf-style) to the StringBuffer
 * @param self StringBuffer object instance
 * @param format printf-style format string
 * @param ... Format arguments
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferAppendFormat(void* self, const char* format, ...);

/* ----------------- Insertion Functions ----------------- */

/**
 * @brief Inserts a String object at the specified position
 * @param self StringBuffer object instance
 * @param offset Position at which to insert
 * @param string String object to insert
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferInsertString(void* self, size_t offset, const void* string);

/**
 * @brief Inserts a C string at the specified position
 * @param self StringBuffer object instance
 * @param offset Position at which to insert
 * @param str Null-terminated C string to insert
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferInsertCString(void* self, size_t offset, const char* str);

/**
 * @brief Inserts a character at the specified position
 * @param self StringBuffer object instance
 * @param offset Position at which to insert
 * @param ch Character to insert
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferInsertChar(void* self, size_t offset, char ch);

/**
 * @brief Inserts an integer at the specified position
 * @param self StringBuffer object instance
 * @param offset Position at which to insert
 * @param value Integer value to insert
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferInsertInt(void* self, size_t offset, int value);

/**
 * @brief Inserts a double at the specified position
 * @param self StringBuffer object instance
 * @param offset Position at which to insert
 * @param value Double value to insert
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferInsertDouble(void* self, size_t offset, double value);

/**
 * @brief Inserts a boolean at the specified position
 * @param self StringBuffer object instance
 * @param offset Position at which to insert
 * @param value Boolean value to insert (will insert "true" or "false")
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferInsertBool(void* self, size_t offset, bool value);

/**
 * @brief Inserts any object at the specified position by calling its toString method
 * @param self StringBuffer object instance
 * @param offset Position at which to insert
 * @param object Object to insert (must have a toString method)
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferInsertObject(void* self, size_t offset, const void* object);

/**
 * @brief Inserts formatted text at a specified position
 * @param self StringBuffer object instance
 * @param offset Insertion position
 * @param format printf-style format string
 * @param ... Format arguments
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferInsertFormat(void* self, size_t offset, const char* format, ...);

/* ----------------- Deletion Functions ----------------- */

/**
 * @brief Deletes characters from the StringBuffer
 * @param self StringBuffer object instance
 * @param start Starting index (inclusive)
 * @param end Ending index (exclusive)
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferDelete(void* self, size_t start, size_t end);

/**
 * @brief Deletes a single character at the specified index
 * @param self StringBuffer object instance
 * @param index Index of the character to delete
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferDeleteCharAt(void* self, size_t index);

/* ----------------- Utility Functions ----------------- */

/**
 * @brief Reverses the character sequence in the StringBuffer
 * @param self StringBuffer object instance
 * @return Pointer to self for method chaining, or NULL on failure
 */
void* ooc_stringBufferReverse(void* self);

/**
 * @brief Clears all content from the StringBuffer
 * @param self StringBuffer object instance
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_stringBufferClear(void* self);

/* ----------------- Search Functions ----------------- */

/**
 * @brief Returns the index of the first occurrence of a character
 * @param self StringBuffer object instance
 * @param ch Character to search for
 * @return Zero-based index of the first occurrence, or -1 if not found
 */
int ooc_stringBufferIndexOfChar(const void* self, char ch);

/**
 * @brief Returns the index of the last occurrence of a character
 * @param self StringBuffer object instance
 * @param ch Character to search for
 * @return Zero-based index of the last occurrence, or -1 if not found
 */
int ooc_stringBufferLastIndexOfChar(const void* self, char ch);

/**
 * @brief Returns the index of the first occurrence of a substring (String object)
 * @param self StringBuffer object instance
 * @param str Substring to search for
 * @return Zero-based index of the first occurrence, or -1 if not found
 */
int ooc_stringBufferIndexOfString(const void* self, const void* str);

/**
 * @brief Returns the index of the first occurrence of a C string
 * @param self StringBuffer object instance
 * @param str Null-terminated C string to search for
 * @return Zero-based index of the first occurrence, or -1 if not found
 */
int ooc_stringBufferIndexOfCString(const void* self, const char* str);

/**
 * @brief Returns the index of the last occurrence of a substring (String object)
 * @param self StringBuffer object instance
 * @param str Substring to search for
 * @return Zero-based index of the last occurrence, or -1 if not found
 */
int ooc_stringBufferLastIndexOfString(const void* self, const void* str);

/**
 * @brief Returns the index of the last occurrence of a C string
 * @param self StringBuffer object instance
 * @param str Null-terminated C string to search for
 * @return Zero-based index of the last occurrence, or -1 if not found
 */
int ooc_stringBufferLastIndexOfCString(const void* self, const char* str);

/* ----------------- Prefix/Suffix/Contains ----------------- */

/**
 * @brief Checks if the StringBuffer starts with a prefix (String object)
 * @param self StringBuffer object instance
 * @param prefix Prefix string to check
 * @return true if it starts with the prefix, false otherwise
 */
bool ooc_stringBufferStartsWithString(const void* self, const void* prefix);

/**
 * @brief Checks if the StringBuffer starts with a C string
 * @param self StringBuffer object instance
 * @param prefix Null-terminated prefix string to check
 * @return true if it starts with the prefix, false otherwise
 */
bool ooc_stringBufferStartsWithCString(const void* self, const char* prefix);

/**
 * @brief Checks if the StringBuffer ends with a suffix (String object)
 * @param self StringBuffer object instance
 * @param suffix Suffix string to check
 * @return true if it ends with the suffix, false otherwise
 */
bool ooc_stringBufferEndsWithString(const void* self, const void* suffix);

/**
 * @brief Checks if the StringBuffer ends with a C string
 * @param self StringBuffer object instance
 * @param suffix Null-terminated suffix string to check
 * @return true if it ends with the suffix, false otherwise
 */
bool ooc_stringBufferEndsWithCString(const void* self, const char* suffix);

/**
 * @brief Checks if the StringBuffer contains a substring (String object)
 * @param self StringBuffer object instance
 * @param str Substring to check
 * @return true if the substring is present, false otherwise
 */
bool ooc_stringBufferContainsString(const void* self, const void* str);

/**
 * @brief Checks if the StringBuffer contains a C string
 * @param self StringBuffer object instance
 * @param str Null-terminated substring to check
 * @return true if the substring is present, false otherwise
 */
bool ooc_stringBufferContainsCString(const void* self, const char* str);

/* ----------------- Conversion Functions ----------------- */

/**
 * @brief Converts the StringBuffer to a newly allocated C string
 * @param self StringBuffer object instance
 * @return Newly allocated C string copy, or NULL on failure
 * @note Caller is responsible for freeing the returned string
 */
char* ooc_stringBufferToCString(const void* self);

/**
 * @brief Returns a const pointer to the internal C string (read-only access)
 * @param self StringBuffer object instance
 * @return Const pointer to the internal C string, or NULL if self is NULL
 * @note The returned pointer should not be modified or freed.
 *       The pointer may become invalid after any operation that modifies the StringBuffer.
 */
const char* ooc_stringBufferGetCString(const void* self);

/**
 * @brief Converts the StringBuffer to a String object
 * @param self StringBuffer object instance
 * @return New String object, or NULL on failure
 * @note Caller is responsible for destroying the returned String object with ooc_destroy
 */
void* ooc_stringBufferToString(const void* self);

#endif /* OOC_STRING_BUFFER_H_ */

