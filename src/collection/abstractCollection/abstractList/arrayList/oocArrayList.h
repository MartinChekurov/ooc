/**
 * @file oocArrayList.h
 * @brief ArrayList implementation for the OOC (Object-Oriented C) framework
 *
 * This header defines the ArrayList class, a resizable array implementation
 * of the List interface. It provides fast indexed access and amortized O(1)
 * append operations.
 */

#ifndef OOC_ARRAY_LIST_H_
#define OOC_ARRAY_LIST_H_

#include "oocError.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Returns the ArrayList class descriptor
 * @return Pointer to the ArrayList class object
 *
 * Constructor parameters:
 * - initialCapacity (size_t): Initial capacity for the array, or 0 for default (10)
 *
 * Example:
 * @code
 *   void* list = ooc_new(ooc_arrayListClass(), 20);  // Create with capacity 20
 * @endcode
 */
void* ooc_arrayListClass();

/**
 * @brief Returns the current capacity of the ArrayList
 * @param self ArrayList object instance
 * @return Current capacity, or 0 if self is NULL
 */
size_t ooc_arrayListCapacity(void* self);

/**
 * @brief Ensures the ArrayList can hold at least the specified capacity
 * @param self ArrayList object instance
 * @param minCapacity Minimum capacity required
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_arrayListEnsureCapacity(void* self, size_t minCapacity);

/**
 * @brief Trims the capacity to match the current size
 * @param self ArrayList object instance
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_arrayListTrimToSize(void* self);

void* ooc_arrayListGetIterator(void* self);
size_t ooc_arrayListSize(void* self);
bool ooc_arrayListIsEmpty(void* self);
bool ooc_arrayListContains(void* self, void* element);
bool ooc_arrayListContainsAll(void* self, void* other);
OOC_Error ooc_arrayListAdd(void* self, void* element);
OOC_Error ooc_arrayListRemove(void* self, void* element);
OOC_Error ooc_arrayListClear(void* self);
void* ooc_arrayListGetAt(void* self, size_t index);
OOC_Error ooc_arrayListSetAt(void* self, size_t index, void* element);
OOC_Error ooc_arrayListInsertAt(void* self, size_t index, void* element);
OOC_Error ooc_arrayListRemoveAt(void* self, size_t index);
int ooc_arrayListIndexOf(void* self, void* element);
int ooc_arrayListLastIndexOf(void* self, void* element);
void* ooc_arrayListGetListIterator(void* self);
void* ooc_arrayListGetListIteratorAt(void* self, size_t index);

#endif
