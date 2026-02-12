/**
 * @file oocDeque.h
 * @brief Deque (Double-Ended deque) interface for the OOC framework
 *
 * This header defines the Deque interface that represents a linear collection
 * supporting element insertion and removal at both ends. The name deque is
 * short for "double ended deque" and is usually pronounced "deck".
 *
 * Deque extends the deque interface and provides methods to insert, remove,
 * and examine elements at both the head and tail of the deque.
 */

#ifndef OOC_DEQUE_H_
#define OOC_DEQUE_H_

#include "oocError.h"
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Returns the Deque interface class descriptor
 * @return Pointer to the Deque interface class object
 *
 * This function returns the class descriptor for the Deque interface,
 * which extends deque and adds double-ended operations.
 */
void* ooc_dequeClass();

/**
 * @brief Returns an Iterator over the elements in deque
 * @param self Iterable object instance
 * @return Pointer to a new Iterator object, or NULL on failure
 * @note Caller is responsible for calling ooc_destroy() on the returned iterator
 * @note Multiple iterators can be obtained and used concurrently, each maintaining
 *       its own iteration state
 *
 * This method creates a new Iterator that can be used to traverse the elements
 * of the deque. The Iterator interface provides hasNext() and next() methods
 * for sequential access.
 *
 * Example usage:
 * @code
 *   void* iter = ooc_dequeGetIterator(collection);
 *   while (ooc_iteratorHasNext(iter)) {
 *       void* element = ooc_iteratorNext(iter);
 *       // process element
 *   }
 *   ooc_destroy(iter);
 * @endcode
 */
void* ooc_dequeGetIterator(void* self);

/**
 * @brief Returns the number of elements in the deque
 * @param self deque object instance
 * @return Number of elements, or 0 if self is NULL
 */
size_t ooc_dequeSize(void* self);

/**
 * @brief Checks if the deque is empty
 * @param self deque object instance
 * @return true if the deque contains no elements, false otherwise
 */
bool ooc_dequeIsEmpty(void* self);

/**
 * @brief Checks if the deque contains the specified element
 * @param self deque object instance
 * @param element Element to check for presence
 * @return true if the deque contains the element, false otherwise
 */
bool ooc_dequeContains(void* self, void* element);

/**
 * @brief Checks if this deque contains all elements from another deque
 * @param self Deque object instance
 * @param other Deque to be checked for containment
 * @return true if this deque contains all elements of the specified deque
 */
bool ooc_dequeContainsAll(void* self, void* other);

/**
 * @brief Adds an element to the tail (back/end) of the deque
 * @param self deque object instance
 * @param element Element to add
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note Inherited from Collection interface. Equivalent to addLast()
 */
OOC_Error ooc_dequeAdd(void* self, void* element);

/**
 * @brief Removes the first occurrence of the specified element from the deque
 * @param self deque object instance
 * @param element Element to remove
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note Inherited from Collection interface
 */
OOC_Error ooc_dequeRemove(void* self, void* element);

/**
 * @brief Removes all elements from the deque
 * @param self deque object instance
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_dequeClear(void* self);

/**
 * @brief Inserts an element at the tail (back/end) of the deque
 * @param self deque object instance
 * @param element Element to add
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note Inherited from Queue interface. Equivalent to addLast()
 */
OOC_Error ooc_dequeOffer(void* self, void* element);

/**
 * @brief Retrieves and removes the head (front) element of the deque
 * @param self deque object instance
 * @return Pointer to the removed element, or NULL if deque is empty
 * @note Inherited from Queue interface. Equivalent to removeFirst()
 */
void* ooc_dequePoll(void* self);

/**
 * @brief Retrieves, but does not remove, the head (front) element of the deque
 * @param self deque object instance
 * @return Pointer to the head element, or NULL if deque is empty
 * @note Inherited from Queue interface. Equivalent to getFirst()
 */
void* ooc_dequePeek(void* self);

/**
 * @brief Inserts an element at the head (front) of the deque
 * @param self Deque object instance
 * @param element Element to add
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note Adds to the front of the deque
 */
OOC_Error ooc_dequeAddFirst(void* self, void* element);

/**
 * @brief Inserts an element at the tail (back/end) of the deque
 * @param self Deque object instance
 * @param element Element to add
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 * @note Adds to the back of the deque. Same as add() and push()
 */
OOC_Error ooc_dequeAddLast(void* self, void* element);

/**
 * @brief Retrieves and removes the head (front) element of the deque
 * @param self Deque object instance
 * @return Pointer to the removed element, or NULL if deque is empty
 * @note Removes from the front. Same as pop()
 */
void* ooc_dequeRemoveFirst(void* self);

/**
 * @brief Retrieves and removes the tail (back/end) element of the deque
 * @param self Deque object instance
 * @return Pointer to the removed element, or NULL if deque is empty
 * @note Removes from the back
 */
void* ooc_dequeRemoveLast(void* self);

/**
 * @brief Retrieves, but does not remove, the head (front) element of the deque
 * @param self Deque object instance
 * @return Pointer to the first element, or NULL if deque is empty
 * @note Returns the front element. Same as peek()
 */
void* ooc_dequeGetFirst(void* self);

/**
 * @brief Retrieves, but does not remove, the tail (back/end) element of the deque
 * @param self Deque object instance
 * @return Pointer to the last element, or NULL if deque is empty
 * @note Returns the back element
 */
void* ooc_dequeGetLast(void* self);

OOC_Error ooc_dequePush(void* self, void* element);
void* ooc_dequeGetPop(void* self);

#endif
