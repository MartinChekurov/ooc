#ifndef OOC_ABSTRACT_SEQUENTIAL_LIST_H_
#define OOC_ABSTRACT_SEQUENTIAL_LIST_H_

#include "oocError.h"
#include <stdbool.h>
#include <stddef.h>

void* ooc_abstractSequentialListClass(void);

void* ooc_abstractSequentialListGetIterator(void* self);
size_t ooc_abstractSequentialListSize(void* self);
bool ooc_abstractSequentialListIsEmpty(void* self);
bool ooc_abstractSequentialListContains(void* self, void* element);
bool ooc_abstractSequentialListContainsAll(void* self, void* other);
OOC_Error ooc_abstractSequentialListAdd(void* self, void* element);
OOC_Error ooc_abstractSequentialListRemove(void* self, void* element);
OOC_Error ooc_abstractSequentialListClear(void* self);
void* ooc_abstractSequentialListGetAt(void* self, size_t index);
OOC_Error ooc_abstractSequentialListSetAt(void* self, size_t index, void* element);
OOC_Error ooc_abstractSequentialListInsertAt(void* self, size_t index, void* element);
OOC_Error ooc_abstractSequentialListRemoveAt(void* self, size_t index);
int ooc_abstractSequentialListIndexOf(void* self, void* element);
int ooc_abstractSequentialListLastIndexOf(void* self, void* element);
void* ooc_abstractSequentialListGetListIterator(void* self);
void* ooc_abstractSequentialListGetListIteratorAt(void* self, size_t index);

#endif /* OOC_ABSTRACT_SEQUENTIAL_LIST_H_ */
