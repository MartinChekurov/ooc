#ifndef OOC_ABSTRACT_ITERATOR_H_
#define OOC_ABSTRACT_ITERATOR_H_

#include "oocError.h"
#include <stdbool.h>

void* ooc_abstractIteratorClass(void);

bool ooc_abstractIteratorHasNext(void* self);
void* ooc_abstractIteratorNext(void* self);
OOC_Error ooc_abstractIteratorRemove(void* self);

#endif
