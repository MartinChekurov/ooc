/**
 * @file oocLinkedHashMap.h
 * @brief LinkedHashMap implementation for the OOC (Object-Oriented C) framework
 *
 * This header defines the LinkedHashMap class that implements the Map interface
 * using a hash table with separate chaining and a doubly-linked list running through
 * all entries to maintain insertion order. This implementation permits null keys and null values.
 * Performance: O(1) average case for get, put, and remove operations.
 * Iteration order: Entries are returned in the order they were inserted.
 */

#ifndef OOC_LINKED_HASH_MAP_H_
#define OOC_LINKED_HASH_MAP_H_

/**
 * @brief Returns the LinkedHashMap class descriptor
 * @return Pointer to the LinkedHashMap class object
 *
 * Constructor parameters:
 * - initialCapacity (size_t): Initial capacity for the hash table, or 0 for default (16)
 *
 * Example:
 * @code
 *   void* map = ooc_new(ooc_linkedHashMapClass(), 16);
 * @endcode
 */
void* ooc_linkedHashMapClass();

/**
 * @brief Returns an iterator over the keys contained in this LinkedHashMap
 *
 * The returned iterator traverses the keys in the order in which they were
 * inserted into the map.
 *
 * The iterator reflects the state of the map at the time of creation.
 * Structural modification of the map during iteration (except through
 * the iterator’s own remove operation, if supported) results in undefined behavior.
 *
 * Ownership:
 * - The caller owns the returned iterator and is responsible for destroying it
 *   using ooc_destroy().
 * - The map owns the keys returned by the iterator.
 *
 * @param self Pointer to a LinkedHashMap instance
 * @return Pointer to an Iterator over the map’s keys in insertion order, or NULL on error
 *
 * Example:
 * @code
 *   void* it = ooc_linkedHashMapGetKeyIterator(map);
 *   while (ooc_iteratorHasNext(it)) {
 *       void* key = ooc_iteratorNext(it);
 *       // use key
 *   }
 *   ooc_destroy(it);
 * @endcode
 */
void* ooc_linkedHashMapGetKeyIterator(void* self);

#endif
