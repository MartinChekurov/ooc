/**
 * @file oocHashSet.h
 * @brief HashSet implementation for the OOC (Object-Oriented C) framework
 *
 * This header defines the HashSet class that implements the Set interface
 * using a hash table with separate chaining. It makes no guarantees about
 * the iteration order of elements. This implementation permits the null element.
 * Performance: O(1) average case for add, remove, and contains operations.
 */

#ifndef OOC_HASH_SET_H_
#define OOC_HASH_SET_H_

/**
 * @brief Returns the HashSet class descriptor
 * @return Pointer to the HashSet class object
 *
 * Constructor parameters:
 * - initialCapacity (size_t): Initial capacity for the hash table, or 0 for default (16)
 *
 * Example:
 * @code
 *   void* set = ooc_new(ooc_hashSetClass(), 16);
 * @endcode
 */
void* ooc_hashSetClass();

#endif
