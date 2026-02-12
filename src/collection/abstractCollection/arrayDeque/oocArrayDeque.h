/**
 * @file oocArrayDeque.h
 * @brief ArrayDeque concrete class for OOC.
 *
 * Java-style design:
 * - ArrayDeque extends AbstractCollection
 * - Implements Deque (and therefore Queue/Collection/Iterable)
 * - Backed by a circular resizable array
 *
 * Notes aligned with java.util.ArrayDeque semantics:
 * - null elements are not allowed
 * - add/addLast append at tail
 * - addFirst prepends at head
 * - remove(Object) removes first matching occurrence
 */

#ifndef OOC_ARRAY_DEQUE_H_
#define OOC_ARRAY_DEQUE_H_

#include "oocError.h"
#include <stdbool.h>
#include <stddef.h>

void* ooc_arrayDequeClass(void);

void* ooc_arrayDequeGetIterator(void* self);
size_t ooc_arrayDequeSize(void* self);
bool ooc_arrayDequeIsEmpty(void* self);
bool ooc_arrayDequeContains(void* self, void* element);
bool ooc_arrayDequeContainsAll(void* self, void* other);
OOC_Error ooc_arrayDequeAdd(void* self, void* element);
OOC_Error ooc_arrayDequeRemove(void* self, void* element);
OOC_Error ooc_arrayDequeClear(void* self);

OOC_Error ooc_arrayDequeOffer(void* self, void* element);
void* ooc_arrayDequePoll(void* self);
void* ooc_arrayDequePeek(void* self);

OOC_Error ooc_arrayDequeAddFirst(void* self, void* element);
OOC_Error ooc_arrayDequeAddLast(void* self, void* element);
void* ooc_arrayDequeRemoveFirst(void* self);
void* ooc_arrayDequeRemoveLast(void* self);
void* ooc_arrayDequeGetFirst(void* self);
void* ooc_arrayDequeGetLast(void* self);
OOC_Error ooc_arrayDequePush(void* self, void* element);
void* ooc_arrayDequePop(void* self);

#endif /* OOC_ARRAY_DEQUE_H_ */
