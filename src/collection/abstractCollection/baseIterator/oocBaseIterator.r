#ifndef OOC_BASE_ITERATOR_R_
#define OOC_BASE_ITERATOR_R_

#include "oocObject.r"
#include "oocIterator.r"
#include <stdbool.h>

typedef struct OOC_BaseIterator OOC_BaseIterator;
typedef struct OOC_BaseIteratorClass OOC_BaseIteratorClass;

struct OOC_BaseIterator {
    OOC_Object object;
    bool canRemove;
};

struct OOC_BaseIteratorClass {
    OOC_Class class;
    OOC_IteratorVtable iterator;
};

bool ooc_superBaseIteratorHasNext(void* self);
void* ooc_superBaseIteratorNext(void* self);
OOC_Error ooc_superBaseIteratorRemove(void* self);

#define OOC_BASE_ITERATOR_METHOD_HAS_NEXT OOC_METHOD(OOC_BaseIteratorClass, iterator.hasNext)
#define OOC_BASE_ITERATOR_METHOD_NEXT     OOC_METHOD(OOC_BaseIteratorClass, iterator.next)
#define OOC_BASE_ITERATOR_METHOD_REMOVE   OOC_METHOD(OOC_BaseIteratorClass, iterator.remove)

#endif
