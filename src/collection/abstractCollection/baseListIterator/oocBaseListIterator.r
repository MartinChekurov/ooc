#ifndef OOC_BASE_LIST_ITERATOR_R_
#define OOC_BASE_LIST_ITERATOR_R_

#include "oocBaseIterator.r"
#include "oocListIterator.r"
#include <stddef.h>

typedef struct OOC_BaseListIterator OOC_BaseListIterator;
typedef struct OOC_BaseListIteratorClass OOC_BaseListIteratorClass;

struct OOC_BaseListIterator {
    OOC_BaseIterator baseIterator;
};

struct OOC_BaseListIteratorClass {
    OOC_BaseIteratorClass class;
    OOC_ListIteratorVtable listIteratorVtable;
};

#define OOC_BASE_LIST_ITERATOR_METHOD_HAS_PREVIOUS \
    offsetof(OOC_BaseListIteratorClass, listIteratorVtable.hasPrevious)
#define OOC_BASE_LIST_ITERATOR_METHOD_PREVIOUS \
    offsetof(OOC_BaseListIteratorClass, listIteratorVtable.previous)
#define OOC_BASE_LIST_ITERATOR_METHOD_NEXT_INDEX \
    offsetof(OOC_BaseListIteratorClass, listIteratorVtable.nextIndex)
#define OOC_BASE_LIST_ITERATOR_METHOD_PREVIOUS_INDEX \
    offsetof(OOC_BaseListIteratorClass, listIteratorVtable.previousIndex)
#define OOC_BASE_LIST_ITERATOR_METHOD_SET \
    offsetof(OOC_BaseListIteratorClass, listIteratorVtable.set)
#define OOC_BASE_LIST_ITERATOR_METHOD_ADD \
    offsetof(OOC_BaseListIteratorClass, listIteratorVtable.add)

#endif
