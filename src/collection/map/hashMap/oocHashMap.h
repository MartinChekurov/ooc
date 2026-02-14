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

#include "oocError.h"
#include <stdbool.h>
#include <stddef.h>

void* ooc_hashMapClass(void);

OOC_Error ooc_hashMapPut(void* self, void* key, void* value);

size_t ooc_hashMapSize(void* self);
bool ooc_hashMapIsEmpty(void* self);
bool ooc_hashMapContainsKey(void* self, void* key);
bool ooc_hashMapContainsValue(void* self, void* value);
void* ooc_hashMapGet(void* self, void* key);
OOC_Error ooc_hashMapPut(void* self, void* key, void* value);
OOC_Error ooc_hashMapRemove(void* self, void* key);
OOC_Error ooc_hashMapClear(void* self);
void* ooc_hashMapKeySet(void* self);
void* ooc_hashMapValues(void* self);
void* ooc_hashMapGetIterator(void* self);

#endif
