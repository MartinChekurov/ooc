/**
 * @file oocMap.h
 * @brief Map interface for the OOC (Object-Oriented C) framework
 *
 * This header defines the Map interface that represents a collection of key-value pairs.
 * Maps cannot contain duplicate keys; each key can map to at most one value.
 */

#ifndef OOC_MAP_H_
#define OOC_MAP_H_

#include "oocError.h"
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Returns the Map interface class descriptor
 * @return Pointer to the Map interface class object
 */
void* ooc_mapClass();

/**
 * @brief Returns the number of key-value mappings in the map
 * @param self Map object instance
 * @return Number of mappings, or 0 if self is NULL
 */
size_t ooc_mapSize(void* self);

/**
 * @brief Checks if the map is empty
 * @param self Map object instance
 * @return true if the map contains no key-value mappings, false otherwise
 */
bool ooc_mapIsEmpty(void* self);

/**
 * @brief Checks if the map contains a mapping for the specified key
 * @param self Map object instance
 * @param key Key whose presence in the map is to be tested
 * @return true if the map contains a mapping for the key, false otherwise
 */
bool ooc_mapContainsKey(void* self, void* key);

/**
 * @brief Checks if the map contains one or more keys mapping to the specified value
 * @param self Map object instance
 * @param value Value whose presence in the map is to be tested
 * @return true if the map maps one or more keys to the value, false otherwise
 */
bool ooc_mapContainsValue(void* self, void* value);

/**
 * @brief Returns the value to which the specified key is mapped
 * @param self Map object instance
 * @param key Key whose associated value is to be returned
 * @return The value to which the key is mapped, or NULL if the map contains no mapping for the key
 */
void* ooc_mapGet(void* self, void* key);

/**
 * @brief Associates the specified value with the specified key in the map
 * @param self Map object instance
 * @param key Key with which the value is to be associated
 * @param value Value to be associated with the key
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_mapPut(void* self, void* key, void* value);

/**
 * @brief Removes the mapping for a key from the map if it is present
 * @param self Map object instance
 * @param key Key whose mapping is to be removed from the map
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_mapRemove(void* self, void* key);

/**
 * @brief Removes all of the mappings from the map
 * @param self Map object instance
 * @return OOC_ERROR_NONE on success, appropriate error code on failure
 */
OOC_Error ooc_mapClear(void* self);

/**
 * @brief Returns a Set view of the keys contained in the map
 * @param self Map object instance
 * @return Set object containing all keys, or NULL on failure
 * @note Caller is responsible for destroying the returned Set object with ooc_destroy
 */
void* ooc_mapKeySet(void* self);

/**
 * @brief Returns a Collection view of the values contained in the map
 * @param self Map object instance
 * @return Collection object containing all values, or NULL on failure
 * @note Caller is responsible for destroying the returned Collection object with ooc_destroy
 */
void* ooc_mapValues(void* self);

/**
 * @brief Returns an iterator over the entries in the map
 * @param self Map object instance
 * @return Iterator object, or NULL on failure
 * @note Each call to ooc_iteratorNext returns a HashMapEntry. Caller must destroy the iterator.
 */
void* ooc_mapGetIterator(void* self);

#endif
