#ifndef OOC_ABSTRACT_ITERATOR_R_
#define OOC_ABSTRACT_ITERATOR_R_

#include "oocObject.r"
#include "oocIterator.r"
#include <stdbool.h>

typedef struct OOC_AbstractIterator OOC_AbstractIterator;
typedef struct OOC_AbstractIteratorClass OOC_AbstractIteratorClass;

struct OOC_AbstractIterator {
    OOC_Object object;
    bool canModify;
};

struct OOC_AbstractIteratorClass {
    OOC_Class class;
    OOC_IteratorVtable iterator;
};

#define OOC_ABSTRACT_ITERATOR_METHOD_HAS_NEXT OOC_METHOD(OOC_AbstractIteratorClass, iterator.hasNext)
#define OOC_ABSTRACT_ITERATOR_METHOD_NEXT     OOC_METHOD(OOC_AbstractIteratorClass, iterator.next)
#define OOC_ABSTRACT_ITERATOR_METHOD_REMOVE   OOC_METHOD(OOC_AbstractIteratorClass, iterator.remove)

#endif
