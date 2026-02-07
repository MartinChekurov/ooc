/**
 * @file oocArrayDeque.h
 * @brief ArrayDeque implementation for the OOC (Object-Oriented C) framework
 *
 * This header defines the ArrayDeque class, a resizable array implementation
 * of the Deque interface. It provides efficient O(1) insertion and deletion
 * operations at both ends using a circular buffer.
 */

#ifndef OOC_ARRAY_DEQUE_H_
#define OOC_ARRAY_DEQUE_H_

/**
 * @brief Returns the ArrayDeque class descriptor
 * @return Pointer to the ArrayDeque class object
 *
 * Constructor parameters:
 * - initialCapacity (size_t): Initial capacity for the circular buffer, or 0 for default (16)
 *
 * Example:
 * @code
 *   void* deque = ooc_new(ooc_arrayDequeClass(), 32);  // Create with capacity 32
 * @endcode
 */
void* ooc_arrayDequeClass();

#endif
