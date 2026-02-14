/**
 * @file oocAbstractMap.h
 * @brief Abstract base class for Map implementations in the OOC framework
 *
 * This header defines the AbstractMap class that provides a skeletal
 * implementation of the Map interface, to minimize the effort required
 * to implement this interface. This class is abstract and cannot be
 * instantiated directly.
 */

#ifndef OOC_ABSTRACT_MAP_H_
#define OOC_ABSTRACT_MAP_H_

#include "oocError.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Returns the AbstractMap class descriptor
 * @return Pointer to the AbstractMap class object
 */
void* ooc_abstractMapClass(void);

size_t ooc_abstractMapSize(void* self);
bool ooc_abstractMapIsEmpty(void* self);
bool ooc_abstractMapContainsKey(void* self, void* key);
bool ooc_abstractMapContainsValue(void* self, void* value);
void* ooc_abstractMapGet(void* self, void* key);
OOC_Error ooc_abstractMapPut(void* self, void* key, void* value);
OOC_Error ooc_abstractMapRemove(void* self, void* key);
OOC_Error ooc_abstractMapClear(void* self);
void* ooc_abstractMapKeySet(void* self);
void* ooc_abstractMapValues(void* self);
void* ooc_abstractMapGetIterator(void* self);

#endif
