#ifndef OOC_LINKED_HASH_SET_R_
#define OOC_LINKED_HASH_SET_R_

#include "oocHashSet.r"

typedef struct OOC_LinkedHashSet OOC_LinkedHashSet;
typedef struct OOC_LinkedHashSetClass OOC_LinkedHashSetClass;

struct OOC_LinkedHashSet {
    OOC_HashSet object;
    void* map;
};

struct OOC_LinkedHashSetClass {
    OOC_HashSetClass class;
};

#endif
