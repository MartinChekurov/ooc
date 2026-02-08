/**
 * @file oocAbstractList.h
 * @brief Abstract base class for List implementations in the OOC framework
 *
 * Similar to java.util.AbstractList, this class provides skeletal List
 * behavior on top of random-access primitives. Subclasses should implement at
 * least size() and getAt(index). For modifiable lists they should additionally
 * override setAt(), insertAt(), and removeAt().
 */

#ifndef OOC_ABSTRACT_LIST_H_
#define OOC_ABSTRACT_LIST_H_

#include "oocError.h"
#include <stddef.h>
#include <stdbool.h>

void* ooc_abstractListClass(void);

void* ooc_abstractListGetIterator(void* self);
size_t ooc_abstractListSize(void* self);
bool ooc_abstractListIsEmpty(void* self);
bool ooc_abstractListContains(void* self, void* element);
bool ooc_abstractListContainsAll(void* self, void* other);
OOC_Error ooc_abstractListAdd(void* self, void* element);
OOC_Error ooc_abstractListRemove(void* self, void* element);
OOC_Error ooc_abstractListClear(void* self);
void* ooc_abstractListGetAt(void* self, size_t index);
OOC_Error ooc_abstractListSetAt(void* self, size_t index, void* element);
OOC_Error ooc_abstractListInsertAt(void* self, size_t index, void* element);
OOC_Error ooc_abstractListRemoveAt(void* self, size_t index);
int ooc_abstractListIndexOf(void* self, void* element);
int ooc_abstractListLastIndexOf(void* self, void* element);
void* ooc_abstractListGetListIterator(void* self);
void* ooc_abstractListGetListIteratorAt(void* self, size_t index);

#endif /* OOC_ABSTRACT_LIST_H_ */
