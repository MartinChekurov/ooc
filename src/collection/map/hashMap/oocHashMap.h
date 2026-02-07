/**
 * @file oocHashMap.h
 * @brief HashMap implementation for the OOC (Object-Oriented C) framework
 *
 * This header defines the HashMap class that implements the Map interface
 * using a hash table with separate chaining. It makes no guarantees about
 * the iteration order of entries. This implementation permits null keys and null values.
 * Performance: O(1) average case for get, put, and remove operations.
 */

#ifndef OOC_HASH_MAP_H_
#define OOC_HASH_MAP_H_

/**
 * @brief Returns the HashMap class descriptor
 * @return Pointer to the HashMap class object
 *
 * Constructor parameters:
 * - initialCapacity (size_t): Initial capacity for the hash table, or 0 for default (16)
 *
 * Example:
 * @code
 *   void* map = ooc_new(ooc_hashMapClass(), 16);
 * @endcode
 */
void* ooc_hashMapClass();

/**
 * @brief Returns an iterator over the keys contained in this HashMap
 *
 * The returned iterator traverses all keys currently stored in the map.
 * No guarantees are made about the order of iteration.
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
 * @param self Pointer to a HashMap instance
 * @return Pointer to an Iterator over the map’s keys, or NULL on error
 *
 * Example:
 * @code
 *   void* it = ooc_hashMapGetKeyIterator(map);
 *   while (ooc_iteratorHasNext(it)) {
 *       void* key = ooc_iteratorNext(it);
 *       // use key
 *   }
 *   ooc_destroy(it);
 * @endcode
 */
void* ooc_hashMapGetKeyIterator(void* self);

#endif
