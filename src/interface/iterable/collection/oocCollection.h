/**
 * @file oocCollection.h
 * @brief Collection interface for the OOC (Object-Oriented C) framework
 *
 * This header defines the Collection interface that represents a group of objects.
 * Collections extend the Iterable interface, allowing iteration over elements.
 * This interface provides the foundation for various collection types like lists,
 * sets, and other data structures.
 */

#ifndef OOC_COLLECTION_H_
#define OOC_COLLECTION_H_

#include "oocError.h"
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Returns the Collection interface class descriptor
 * @return Pointer to the Collection interface class object
 *
 * This function returns the class descriptor for the Collection interface,
 * which extends Iterable and adds collection-specific operations.
 */
void* ooc_collectionClass();

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
void* ooc_collectionGetIterator(void* self);

/**
 * @brief Returns the number of elements in the collection
 * @param self Collection object instance
 * @return Number of elements, or 0 if self is NULL
 */
size_t ooc_collectionSize(void* self);

/**
 * @brief Checks if the collection is empty
 * @param self Collection object instance
 * @return true if the collection contains no elements, false otherwise
 */
bool ooc_collectionIsEmpty(void* self);

/**
 * @brief Checks if the collection contains the specified element
 * @param self Collection object instance
 * @param element Element to check for presence
 * @return true if the collection contains the element, false otherwise
 */
bool ooc_collectionContains(void* self, void* element);

/**
 * @brief Checks if this collection contains all elements from another collection
 * @param self Collection object instance
 * @param other Collection to be checked for containment
 * @return true if this collection contains all elements of the specified collection
 */
bool ooc_collectionContainsAll(void* self, void* other);

/**
 * @brief Adds an element to the collection
 * @param self Collection object instance
 * @param element Element to add
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note Behavior may vary by implementation (e.g., Sets reject duplicates)
 */
OOC_Error ooc_collectionAdd(void* self, void* element);

/**
 * @brief Removes the specified element from the collection
 * @param self Collection object instance
 * @param element Element to remove
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_collectionRemove(void* self, void* element);

/**
 * @brief Removes all elements from the collection
 * @param self Collection object instance
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_collectionClear(void* self);

#endif
