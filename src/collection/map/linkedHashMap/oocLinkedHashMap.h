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

#include "oocError.h"
#include <stdbool.h>
#include <stddef.h>

void* ooc_linkedHashMapClass();

size_t ooc_linkedHashMapSize(void* self);
bool ooc_linkedHashMapIsEmpty(void* self);
bool ooc_linkedHashMapContainsKey(void* self, void* key);
bool ooc_linkedHashMapContainsValue(void* self, void* value);
void* ooc_linkedHashMapGet(void* self, void* key);
OOC_Error ooc_linkedHashMapPut(void* self, void* key, void* value);
OOC_Error ooc_linkedHashMapRemove(void* self, void* key);
OOC_Error ooc_linkedHashMapClear(void* self);
void* ooc_linkedHashMapKeySet(void* self);
void* ooc_linkedHashMapValues(void* self);
void* ooc_linkedHashMapGetIterator(void* self);

#endif
