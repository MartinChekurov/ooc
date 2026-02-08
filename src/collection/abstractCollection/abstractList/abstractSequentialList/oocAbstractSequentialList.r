#ifndef OOC_ABSTRACT_SEQUENTIAL_LIST_R_
#define OOC_ABSTRACT_SEQUENTIAL_LIST_R_

#include "oocAbstractList.r"
#include "oocListIterator.r"
#include <stddef.h>

typedef struct OOC_AbstractSequentialList OOC_AbstractSequentialList;
typedef struct OOC_AbstractSequentialListClass OOC_AbstractSequentialListClass;

struct OOC_AbstractSequentialList {
    OOC_AbstractList abstractList;
};

struct OOC_AbstractSequentialListClass {
    OOC_AbstractListClass class;
};

#endif /* OOC_ABSTRACT_SEQUENTIAL_LIST_R_ */
