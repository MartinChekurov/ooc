#ifndef OOC_HASHSET_R_
#define OOC_HASHSET_R_

#include "oocAbstractSet.r"

typedef struct OOC_HashSet OOC_HashSet;
typedef struct OOC_HashSetClass OOC_HashSetClass;

struct OOC_HashSet {
    OOC_AbstractSet object;
    void* map;
};

struct OOC_HashSetClass {
    OOC_AbstractSetClass class;
};


#endif
