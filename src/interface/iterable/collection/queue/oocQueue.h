/**
 * @file oocQueue.h
 * @brief Queue interface for the OOC (Object-Oriented C) framework
 *
 * This header defines the Queue interface that represents a collection designed
 * for holding elements prior to processing. Queues typically order elements in
 * a FIFO (first-in-first-out) manner, though other orderings are possible.
 *
 * Queue extends the Collection interface and provides insertion, extraction,
 * and inspection operations.
 */

#ifndef OOC_QUEUE_H_
#define OOC_QUEUE_H_

#include "oocError.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Returns the Queue interface class descriptor
 * @return Pointer to the Queue interface class object
 *
 * This function returns the class descriptor for the Queue interface,
 * which extends Collection and adds queue-specific operations.
 */
void* ooc_queueClass();

/**
 * @brief Returns an Iterator over the elements in queue
 * @param self Iterable object instance
 * @return Pointer to a new Iterator object, or NULL on failure
 * @note Caller is responsible for calling ooc_destroy() on the returned iterator
 * @note Multiple iterators can be obtained and used concurrently, each maintaining
 *       its own iteration state
 *
 * This method creates a new Iterator that can be used to traverse the elements
 * of the queue. The Iterator interface provides hasNext() and next() methods
 * for sequential access.
 *
 * Example usage:
 * @code
 *   void* iter = ooc_queueGetIterator(collection);
 *   while (ooc_iteratorHasNext(iter)) {
 *       void* element = ooc_iteratorNext(iter);
 *       // process element
 *   }
 *   ooc_destroy(iter);
 * @endcode
 */
void* ooc_queueGetIterator(void* self);

/**
 * @brief Returns the number of elements in the queue
 * @param self queue object instance
 * @return Number of elements, or 0 if self is NULL
 */
size_t ooc_queueSize(void* self);

/**
 * @brief Checks if the queue is empty
 * @param self queue object instance
 * @return true if the queue contains no elements, false otherwise
 */
bool ooc_queueIsEmpty(void* self);

/**
 * @brief Checks if the queue contains the specified element
 * @param self queue object instance
 * @param element Element to check for presence
 * @return true if the queue contains the element, false otherwise
 */
bool ooc_queueContains(void* self, void* element);

/**
 * @brief Checks if this queue contains all elements from another queue
 * @param self Queue object instance
 * @param other Queue to be checked for containment
 * @return true if this queue contains all elements of the specified queue
 */
bool ooc_queueContainsAll(void* self, void* other);

/**
 * @brief Adds an element to the queue
 * @param self queue object instance
 * @param element Element to add
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note Inherited from Collection interface. Behavior depends on implementation.
 */
OOC_Error ooc_queueAdd(void* self, void* element);

/**
 * @brief Removes the first occurrence of the specified element from the queue
 * @param self queue object instance
 * @param element Element to remove
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note Inherited from Collection interface
 */
OOC_Error ooc_queueRemove(void* self, void* element);

/**
 * @brief Removes all elements from the queue
 * @param self queue object instance
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_queueClear(void* self);

/**
 * @brief Inserts an element into the queue
 * @param self Queue object instance
 * @param element Element to add
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note Equivalent to add(). Ordering depends on implementation (FIFO, LIFO, priority, etc.)
 */
OOC_Error ooc_queuePush(void* self, void* element);

/**
 * @brief Retrieves and removes an element from the queue
 * @param self Queue object instance
 * @return Pointer to the removed element, or NULL if queue is empty
 * @note Which element is removed depends on implementation (FIFO, LIFO, priority, etc.)
 */
void* ooc_queuePop(void* self);

/**
 * @brief Retrieves, but does not remove, an element from the queue
 * @param self Queue object instance
 * @return Pointer to the element that would be returned by pop(), or NULL if queue is empty
 * @note Which element is returned depends on implementation
 */
void* ooc_queuePeek(void* self);

#endif
