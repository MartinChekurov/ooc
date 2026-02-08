/**
 * @file oocList.h
 * @brief List interface for the OOC (Object-Oriented C) framework
 *
 * This header defines the List interface that represents an ordered collection
 * with indexed access. Lists extend the Collection interface and provide
 * positional access and manipulation of elements.
 */

#ifndef OOC_LIST_H_
#define OOC_LIST_H_

#include "oocError.h"
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Returns the List interface class descriptor
 * @return Pointer to the List interface class object
 *
 * This function returns the class descriptor for the List interface,
 * which extends Collection and adds indexed access operations.
 */
void* ooc_listClass();

/**
 * @brief Returns an Iterator over the elements in list
 * @param self Iterable object instance
 * @return Pointer to a new Iterator object, or NULL on failure
 * @note Caller is responsible for calling ooc_destroy() on the returned iterator
 * @note Multiple iterators can be obtained and used concurrently, each maintaining
 *       its own iteration state
 *
 * This method creates a new Iterator that can be used to traverse the elements
 * of the list. The Iterator interface provides hasNext() and next() methods
 * for sequential access.
 *
 * Example usage:
 * @code
 *   void* iter = ooc_listGetIterator(collection);
 *   while (ooc_iteratorHasNext(iter)) {
 *       void* element = ooc_iteratorNext(iter);
 *       // process element
 *   }
 *   ooc_destroy(iter);
 * @endcode
 */
void* ooc_listGetIterator(void* self);

/**
 * @brief Returns the number of elements in the list
 * @param self list object instance
 * @return Number of elements, or 0 if self is NULL
 */
size_t ooc_listSize(void* self);

/**
 * @brief Checks if the list is empty
 * @param self list object instance
 * @return true if the list contains no elements, false otherwise
 */
bool ooc_listIsEmpty(void* self);

/**
 * @brief Checks if the list contains the specified element
 * @param self list object instance
 * @param element Element to check for presence
 * @return true if the list contains the element, false otherwise
 */
bool ooc_listContains(void* self, void* element);

/**
 * @brief Checks if this list contains all elements from another list
 * @param self List object instance
 * @param other List to be checked for containment
 * @return true if this list contains all elements of the specified list
 */
bool ooc_listContainsAll(void* self, void* other);

/**
 * @brief Adds an element to the list
 * @param self list object instance
 * @param element Element to add
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_listAdd(void* self, void* element);

/**
 * @brief Removes the specified element from the list
 * @param self list object instance
 * @param element Element to remove
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_listRemove(void* self, void* element);

/**
 * @brief Removes all elements from the list
 * @param self list object instance
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_listClear(void* self);

/**
 * @brief Retrieves the element at the specified index
 * @param self List object instance
 * @param index Zero-based index of the element
 * @return Pointer to the element, or NULL if index is out of bounds
 */
void* ooc_listGetAt(void* self, size_t index);

/**
 * @brief Replaces the element at the specified index
 * @param self List object instance
 * @param index Zero-based index of the element to replace
 * @param element New element to set at the index
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_listSetAt(void* self, size_t index, void* element);

/**
 * @brief Inserts an element at the specified index
 * @param self List object instance
 * @param index Zero-based index where element should be inserted
 * @param element Element to insert
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note Elements at and after the index are shifted to the right
 */
OOC_Error ooc_listInsertAt(void* self, size_t index, void* element);

/**
 * @brief Removes the element at the specified index
 * @param self List object instance
 * @param index Zero-based index of the element to remove
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note Elements after the index are shifted to the left
 */
OOC_Error ooc_listRemoveAt(void* self, size_t index);

/**
 * @brief Finds the first occurrence of an element in the list
 * @param self List object instance
 * @param element Element to search for
 * @return Index of the first occurrence, or -1 if not found
 */
int ooc_listIndexOf(void* self, void* element);

/**
 * @brief Finds the last occurrence of an element in the list
 * @param self List object instance
 * @param element Element to search for
 * @return Index of the last occurrence, or -1 if not found
 */
int ooc_listLastIndexOf(void* self, void* element);

void* ooc_listGetListIterator(void* self);
void* ooc_listGetListIteratorAt(void* self, size_t index);

#endif
