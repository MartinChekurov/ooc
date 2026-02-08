#ifndef OOC_LIST_ITERATOR_H_
#define OOC_LIST_ITERATOR_H_

#include "oocError.h"
#include <stdbool.h>
#include <stddef.h>


void* ooc_listIteratorClass(void);

bool ooc_listIteratorHasNext(void* self);
void* ooc_listIteratorNext(void* self);
bool ooc_listIteratorHasPrevious(void* self);
void* ooc_listIteratorPrevious(void* self);
int ooc_listIteratorNextIndex(void* self);
int ooc_listIteratorPreviousIndex(void* self);
OOC_Error ooc_listIteratorRemove(void* self);
OOC_Error ooc_listIteratorSet(void* self, void* element);
OOC_Error ooc_listIteratorAdd(void* self, void* element);

#endif /* OOC_LIST_ITERATOR_H_ */
