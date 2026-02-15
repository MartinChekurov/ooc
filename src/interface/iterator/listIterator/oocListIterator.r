#ifndef OOC_LIST_ITERATOR_R_
#define OOC_LIST_ITERATOR_R_

#include "oocObject.r"
#include "oocIterator.r"
#include "oocError.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct OOC_ListIteratorVtable OOC_ListIteratorVtable;
typedef struct OOC_ListIteratorClass OOC_ListIteratorClass;


struct OOC_ListIteratorVtable {
    bool      (*hasPrevious)(void* self);
    void*     (*previous)(void* self);
    int       (*nextIndex)(void* self);
    int       (*previousIndex)(void* self);
    OOC_Error (*set)(void* self, void* element);
    OOC_Error (*add)(void* self, void* element);
};

struct OOC_ListIteratorClass {
    OOC_Class class;
    OOC_ListIteratorVtable listIteratorVtable;
};

#define OOC_LIST_ITERATOR_METHOD_HAS_PREVIOUS   offsetof(OOC_ListIteratorClass, listIteratorVtable.hasPrevious)
#define OOC_LIST_ITERATOR_METHOD_PREVIOUS       offsetof(OOC_ListIteratorClass, listIteratorVtable.previous)
#define OOC_LIST_ITERATOR_METHOD_NEXT_INDEX     offsetof(OOC_ListIteratorClass, listIteratorVtable.nextIndex)
#define OOC_LIST_ITERATOR_METHOD_PREVIOUS_INDEX offsetof(OOC_ListIteratorClass, listIteratorVtable.previousIndex)
#define OOC_LIST_ITERATOR_METHOD_SET            offsetof(OOC_ListIteratorClass, listIteratorVtable.set)
#define OOC_LIST_ITERATOR_METHOD_ADD            offsetof(OOC_ListIteratorClass, listIteratorVtable.add)

#endif /* OOC_LIST_ITERATOR_R_ */
