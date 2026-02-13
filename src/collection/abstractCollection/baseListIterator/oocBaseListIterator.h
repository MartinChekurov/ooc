#ifndef OOC_BASE_LIST_ITERATOR_H_
#define OOC_BASE_LIST_ITERATOR_H_

#include "oocError.h"
#include <stdbool.h>

void* ooc_baseListIteratorClass(void);

bool ooc_baseListIteratorHasPrevious(void* self);
void* ooc_baseListIteratorPrevious(void* self);
int ooc_baseListIteratorNextIndex(void* self);
int ooc_baseListIteratorPreviousIndex(void* self);
OOC_Error ooc_baseListIteratorSet(void* self, void* element);
OOC_Error ooc_baseListIteratorAdd(void* self, void* element);

#endif
