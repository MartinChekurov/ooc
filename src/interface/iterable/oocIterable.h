/**
 * @file oocIterable.h
 * @brief Iterable interface for the OOC (Object-Oriented C) framework
 *
 * This header defines the Iterable interface that provides a way to obtain
 * an Iterator for sequential access to elements in a collection.
 */

#ifndef OOC_ITERABLE_H_
#define OOC_ITERABLE_H_

#include "oocError.h"
#include <stdbool.h>

/**
 * @brief Returns the Iterable interface class descriptor
 * @return Pointer to the Iterable interface class object
 *
 * This function returns the class descriptor for the Iterable interface,
 * which can be used to check if an object implements the iterable protocol.
 */
void* ooc_iterableClass();

/**
 * @brief Returns an Iterator over the elements in this collection
 * @param self Iterable object instance
 * @return Pointer to a new Iterator object, or NULL on failure
 * @note Caller is responsible for calling ooc_destroy() on the returned iterator
 * @note Multiple iterators can be obtained and used concurrently, each maintaining
 *       its own iteration state
 *
 * This method creates a new Iterator that can be used to traverse the elements
 * of the collection. The Iterator interface provides hasNext() and next() methods
 * for sequential access.
 *
 * Example usage:
 * @code
 *   void* iter = ooc_iterableGetIterator(collection);
 *   while (ooc_iteratorHasNext(iter)) {
 *       void* element = ooc_iteratorNext(iter);
 *       // process element
 *   }
 *   ooc_destroy(iter);
 * @endcode
 */
void* ooc_iterableGetIterator(void* self);

#endif

