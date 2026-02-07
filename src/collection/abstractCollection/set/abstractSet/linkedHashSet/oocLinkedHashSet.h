/**
 * @file oocLinkedHashSet.h
 * @brief LinkedHashSet implementation for the OOC (Object-Oriented C) framework
 *
 * This header defines the LinkedHashSet class that implements the Set interface
 * using a hash table with separate chaining and a doubly-linked list running through
 * all entries to maintain insertion order. This implementation permits the null element.
 * Performance: O(1) average case for add, remove, and contains operations.
 * Iteration order: Elements are returned in the order they were inserted.
 */

#ifndef OOC_LINKED_HASH_SET_H_
#define OOC_LINKED_HASH_SET_H_

/**
 * @brief Returns the LinkedHashSet class descriptor
 * @return Pointer to the LinkedHashSet class object
 *
 * Constructor parameters:
 * - initialCapacity (size_t): Initial capacity for the hash table, or 0 for default (16)
 *
 * Example:
 * @code
 *   void* set = ooc_new(ooc_linkedHashSetClass(), 16);
 * @endcode
 */
void* ooc_linkedHashSetClass();

#endif
