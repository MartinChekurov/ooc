#ifndef OOC_ABSTRACT_LIST_ITERATOR_H_
#define OOC_ABSTRACT_LIST_ITERATOR_H_

#include "oocError.h"
#include <stdbool.h>

void* ooc_abstractListIteratorClass(void);

bool ooc_abstractListIteratorHasNext(void* self);
void* ooc_abstractListIteratorNext(void* self);
OOC_Error ooc_abstractListIteratorRemove(void* self);
bool ooc_abstractListIteratorHasPrevious(void* self);
void* ooc_abstractListIteratorPrevious(void* self);
int ooc_abstractListIteratorNextIndex(void* self);
int ooc_abstractListIteratorPreviousIndex(void* self);
OOC_Error ooc_abstractListIteratorSet(void* self, void* element);
OOC_Error ooc_abstractListIteratorAdd(void* self, void* element);

#endif
