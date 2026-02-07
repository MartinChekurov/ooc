/**
 * @file oocObject.h
 * @brief Base Object class for the OOC (Object-Oriented C) framework
 * 
 * This header defines the fundamental Object class that serves as the root
 * of the class hierarchy in the OOC framework. It provides basic object
 * operations such as construction, destruction, comparison, hashing, and
 * interface management.
 */

#ifndef OOC_OBJECT_H_
#define OOC_OBJECT_H_

#include "oocError.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Returns the Object class descriptor
 * @return Pointer to the Object class object
 */
void* ooc_objectClass();

// ============================================================================
// Object lifecycle management
// ============================================================================

/**
 * @brief Creates a new instance of the specified class
 * @param class Class descriptor for the object to create
 * @param ... Variable arguments passed to the constructor
 * @return Pointer to the newly created object, or NULL on failure
 * @note The caller is responsible for calling ooc_destroy() when done
 */
void* ooc_new(void* class, ...);

/**
 * @brief Destroys an object and frees its memory
 * @param self Object instance to destroy
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_destroy(void* self);

// ============================================================================
// Class and type information
// ============================================================================

/**
 * @brief Returns the class descriptor of the given object
 * @param self Object instance
 * @return Pointer to the class descriptor, or NULL if self is NULL
 */
const void* ooc_classOf(const void* self);

/**
 * @brief Returns the size in bytes of the given object
 * @param self Object instance
 * @return Size of the object in bytes, or 0 if self is NULL
 */
size_t ooc_sizeOf(const void* self);

/**
 * @brief Checks if an object is an instance of the specified class
 * @param self Object instance to check
 * @param class Class descriptor to check against
 * @return true if self is an instance of class (or a subclass), false otherwise
 */
bool ooc_isInstanceOf(const void* self, const void* class);

/**
 * @brief Returns the class name of the given object
 * @param self Object instance
 * @return Pointer to the class name string, or NULL if self is NULL
 */
const char* ooc_getClassName(const void* self);

/**
 * @brief Checks if the given object's class is abstract
 * @param self Object instance
 * @return true if the class is abstract, false otherwise
 */
bool ooc_isAbstract(const void* self);

/**
 * @brief Checks if the given object's class is final
 * @param self Object instance
 * @return true if the class is final, false otherwise
 */
bool ooc_isFinal(const void* self);

// ============================================================================
// Object comparison and representation
// ============================================================================

/**
 * @brief Returns a string representation of the given object
 * @param self Object instance
 * @return Pointer to a newly allocated string representation, or NULL on failure
 * @note Caller is responsible for freeing the returned string
 */
char* ooc_toString(const void* self);

/**
 * @brief Checks if two objects are equal
 * @param self First object instance
 * @param other Second object instance
 * @return true if the objects are equal, false otherwise
 */
bool ooc_equals(const void* self, const void* other);

/**
 * @brief Computes the hash code for the given object
 * @param self Object instance
 * @return Hash code value for the object
 */
size_t ooc_hashCode(const void* self);

/**
 * @brief Compares two objects
 * @param self First object instance
 * @param other Second object instance
 * @return Negative value if self < other, 0 if equal, positive if self > other
 */
int ooc_compare(const void* self, const void* other);

/**
 * @brief Creates a copy of the given object
 * @param self Object instance to clone
 * @return Pointer to the cloned object, or NULL on failure
 * @note Caller is responsible for calling ooc_destroy() on the clone
 */
void* ooc_clone(const void* self);

// ============================================================================
// Interface management
// ============================================================================

/**
 * @brief Retrieves an interface vtable implementation from the object
 * @param self Object instance
 * @param interfaceType Interface class descriptor
 * @return Pointer to the interface vtable implementation, or NULL if not found
 */
const void* ooc_getInterfaceVtable(const void* self, const void* interfaceType);

/**
 * @brief Sets interfaces to an object
 * @param self Object instance
 * @param impls Array of interface implementation descriptors.
 * @param count Number of interface implementations.
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_setInterface(void* self, const void* impls, unsigned int count);

// ============================================================================
// Function override and verification
// ============================================================================

/**
 * @brief Verifies that all function pointers in a class range are implemented
 * @param self Object instance to verify
 * @param start Start offset in the class structure
 * @param end End offset in the class structure
 * @return OOC_ERROR_NONE if all functions are implemented, error otherwise
 */
OOC_Error ooc_verifyMethods(void* self, size_t start, size_t end);

/**
 * @brief Overrides class method implementations using a va_list
 * @param self Object instance whose class will be modified
 * @param args Variable argument list of offset/function pointer pairs
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_overrideMethods(void* self, va_list* args);

/**
 * @brief Overrides class method implementations using variable arguments
 * @param self Object instance whose class will be modified
 * @param ... Offset/function pointer pairs terminated by 0
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_overrideMethodsv(void* self, ...);

#endif
