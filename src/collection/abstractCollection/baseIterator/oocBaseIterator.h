#ifndef OOC_BASE_ITERATOR_H_
#define OOC_BASE_ITERATOR_H_

#include "oocError.h"

void* ooc_baseIteratorClass(void);

void* ooc_baseIteratorNext(void* self);
OOC_Error ooc_baseIteratorRemove(void* self);

#endif