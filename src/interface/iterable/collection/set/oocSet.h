/**
 * @file oocSet.h
 * @brief Set interface for the OOC (Object-Oriented C) framework
 *
 * This header defines the Set interface that represents a collection containing
 * no duplicate elements. Sets extend the Collection interface and ensure that
 * no two elements e1 and e2 exist such that e1.equals(e2) returns true.
 * At most one null element is allowed.
 */

#ifndef OOC_SET_H_
#define OOC_SET_H_

#include "oocError.h"
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Returns the Set interface class descriptor
 * @return Pointer to the Set interface class object
 *
 * This function returns the class descriptor for the Set interface,
 * which extends Collection and enforces uniqueness of elements.
 */
void* ooc_setClass();

/**
 * @brief Returns an Iterator over the elements in this set
 * @param self Iterable object instance
 * @return Pointer to a new Iterator object, or NULL on failure
 * @note Caller is responsible for calling ooc_destroy() on the returned iterator
 * @note Multiple iterators can be obtained and used concurrently, each maintaining
 *       its own iteration state
 *
 * This method creates a new Iterator that can be used to traverse the elements
 * of the set. The Iterator interface provides hasNext() and next() methods
 * for sequential access.
 *
 * Example usage:
 * @code
 *   void* iter = ooc_setGetIterator(collection);
 *   while (ooc_iteratorHasNext(iter)) {
 *       void* element = ooc_iteratorNext(iter);
 *       // process element
 *   }
 *   ooc_destroy(iter);
 * @endcode
 */
void* ooc_setGetIterator(void* self);

/**
 * @brief Returns the number of elements in the set
 * @param self Set object instance
 * @return Number of elements, or 0 if self is NULL
 */
size_t ooc_setSize(void* self);

/**
 * @brief Checks if the set is empty
 * @param self Set object instance
 * @return true if the set contains no elements, false otherwise
 */
bool ooc_setIsEmpty(void* self);

/**
 * @brief Checks if the set contains the specified element
 * @param self Set object instance
 * @param element Element to check for presence
 * @return true if the set contains the element, false otherwise
 */
bool ooc_setContains(void* self, void* element);

/**
 * @brief Adds an element to the set
 * @param self Set object instance
 * @param element Element to add
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note If the element already exists, no duplicate is added
 */
OOC_Error ooc_setAdd(void* self, void* element);

/**
 * @brief Removes the specified element from the set
 * @param self Set object instance
 * @param element Element to remove
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_setRemove(void* self, void* element);

/**
 * @brief Removes all elements from the set
 * @param self Set object instance
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_setClear(void* self);

#endif
