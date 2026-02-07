#ifndef OOC_ARRAY_LIST_R_
#define OOC_ARRAY_LIST_R_

#include "oocAbstractCollection.r"
#include "oocList.r"
#include "oocError.h"
#include <stddef.h>

typedef struct OOC_ArrayList OOC_ArrayList;
typedef struct OOC_ArrayListClass OOC_ArrayListClass;

struct OOC_ArrayList {
    OOC_AbstractCollection abstractCollection;
    void** elements;
    size_t size;
    size_t capacity;
};

struct OOC_ArrayListClass {
    OOC_AbstractCollectionClass class;
    OOC_ListVtable list;
};

#endif
