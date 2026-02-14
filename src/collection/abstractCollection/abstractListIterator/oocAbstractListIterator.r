#ifndef OOC_ABSTRACT_LIST_ITERATOR_R_
#define OOC_ABSTRACT_LIST_ITERATOR_R_

#include "oocAbstractIterator.r"
#include "oocListIterator.r"
#include <stddef.h>

typedef struct OOC_AbstractListIterator OOC_AbstractListIterator;
typedef struct OOC_AbstractListIteratorClass OOC_AbstractListIteratorClass;

struct OOC_AbstractListIterator {
    OOC_AbstractIterator abstractIterator;
};

struct OOC_AbstractListIteratorClass {
    OOC_AbstractIteratorClass class;
    OOC_ListIteratorVtable listIteratorVtable;
};

#define OOC_ABSTRACT_LIST_ITERATOR_METHOD_HAS_PREVIOUS \
    offsetof(OOC_AbstractListIteratorClass, listIteratorVtable.hasPrevious)
#define OOC_ABSTRACT_LIST_ITERATOR_METHOD_PREVIOUS \
    offsetof(OOC_AbstractListIteratorClass, listIteratorVtable.previous)
#define OOC_ABSTRACT_LIST_ITERATOR_METHOD_NEXT_INDEX \
    offsetof(OOC_AbstractListIteratorClass, listIteratorVtable.nextIndex)
#define OOC_ABSTRACT_LIST_ITERATOR_METHOD_PREVIOUS_INDEX \
    offsetof(OOC_AbstractListIteratorClass, listIteratorVtable.previousIndex)
#define OOC_ABSTRACT_LIST_ITERATOR_METHOD_SET \
    offsetof(OOC_AbstractListIteratorClass, listIteratorVtable.set)
#define OOC_ABSTRACT_LIST_ITERATOR_METHOD_ADD \
    offsetof(OOC_AbstractListIteratorClass, listIteratorVtable.add)

#endif
