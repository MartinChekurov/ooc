/**
 * @file oocIterator.h
 * @brief Iterator interface for the OOC (Object-Oriented C) framework
 *
 * This header defines the Iterator interface used for traversing collections.
 * Iterator objects are obtained from Iterable collections and provide methods
 * to sequentially access elements. Each Iterator maintains its own position state,
 * allowing multiple independent iterations over the same collection.
 */

#ifndef OOC_ITERATOR_H_
#define OOC_ITERATOR_H_

#include "oocError.h"
#include <stdbool.h>

/**
 * @brief Returns the Iterator interface class descriptor
 * @return Pointer to the Iterator interface class object
 *
 * This function returns the class descriptor for the Iterator interface,
 * which can be used to check if an object implements the iteration protocol.
 */
void* ooc_iteratorClass();

/**
 * @brief Checks if there are more elements to iterate
 * @param self Iterator object instance
 * @return true if there are more elements, false otherwise
 */
bool ooc_iteratorHasNext(void* self);

/**
 * @brief Returns the next element and advances the iterator position
 * @param self Iterator object instance
 * @return Pointer to the next element, or NULL if no more elements
 * @note The returned pointer's lifetime depends on the implementation
 */
void* ooc_iteratorNext(void* self);

/**
 * @brief Removes from the underlying collection the last element returned by next()
 * @param self Iterator object instance
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note This method can be called only once per call to next()
 * @note Must call next() at least once before calling remove()
 * @note Calling remove() twice without an intervening next() returns an error
 * @note The behavior is unspecified if the underlying collection is modified
 *       during iteration in any way other than by calling this method
 * @note Not all iterators support removal; unsupported operations return
 *       OOC_ERROR_UNSUPPORTED_OPERATION
 *
 * Example usage:
 * @code
 *   void* iter = ooc_collectionGetIterator(collection);
 *   while (ooc_iteratorHasNext(iter)) {
 *       void* element = ooc_iteratorNext(iter);
 *       if (shouldRemove(element)) {
 *           ooc_iteratorRemove(iter);  // Safe removal during iteration
 *       }
 *   }
 *   ooc_destroy(iter);
 * @endcode
 */
OOC_Error ooc_iteratorRemove(void* self);

#endif
