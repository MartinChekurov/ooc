#ifndef OOC_ARRAY_LIST_R_
#define OOC_ARRAY_LIST_R_

#include "oocAbstractList.r"
#include "oocError.h"
#include <stddef.h>

typedef struct OOC_ArrayList OOC_ArrayList;
typedef struct OOC_ArrayListClass OOC_ArrayListClass;

struct OOC_ArrayList {
    OOC_AbstractList abstractList;
    void** elements;
    size_t size;
    size_t capacity;
};

struct OOC_ArrayListClass {
    OOC_AbstractListClass class;
};

#endif

