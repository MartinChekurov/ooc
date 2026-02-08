/**
 * @file oocLinkedList.h
 * @brief LinkedList concrete class (sequential list) for OOC.
 *
 * Java-style design:
 * - LinkedList extends AbstractSequentialList
 * - Core list primitive exposed by LinkedList is listIteratorAt(index)
 * - Random-access list operations are intended to come from AbstractSequentialList
 */

#ifndef OOC_LINKED_LIST_H_
#define OOC_LINKED_LIST_H_

#include "oocError.h"
#include <stdbool.h>
#include <stddef.h>

void* ooc_linkedListClass(void);

void* ooc_linkedListGetIterator(void* self);
size_t ooc_linkedListSize(void* self);
bool ooc_linkedListIsEmpty(void* self);
bool ooc_linkedListContains(void* self, void* element);
bool ooc_linkedListContainsAll(void* self, void* other);
OOC_Error ooc_linkedListAdd(void* self, void* element);
OOC_Error ooc_linkedListRemove(void* self, void* element);
OOC_Error ooc_linkedListClear(void* self);
void* ooc_linkedListGetAt(void* self, size_t index);
OOC_Error ooc_linkedListSetAt(void* self, size_t index, void* element);
OOC_Error ooc_linkedListInsertAt(void* self, size_t index, void* element);
OOC_Error ooc_linkedListRemoveAt(void* self, size_t index);
int ooc_linkedListIndexOf(void* self, void* element);
int ooc_linkedListLastIndexOf(void* self, void* element);
void* ooc_linkedListGetListIterator(void* self);
void* ooc_linkedListGetListIteratorAt(void* self, size_t index);
OOC_Error ooc_linkedListPush(void* self, void* element);
void* ooc_linkedListPop(void* self);
void* ooc_linkedListPeek(void* self);
OOC_Error ooc_linkedListAddFirst(void* self, void* element);
OOC_Error ooc_linkedListAddLast(void* self, void* element);
void* ooc_linkedListGetFirst(void* self);
void* ooc_linkedListGetLast(void* self);
void* ooc_linkedListRemoveFirst(void* self);
void* ooc_linkedListRemoveLast(void* self);


#endif /* OOC_LINKED_LIST_H_ */
