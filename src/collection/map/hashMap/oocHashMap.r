#ifndef OOC_HASHMAP_R_
#define OOC_HASHMAP_R_

#include "oocAbstractMap.r"

typedef struct OOC_HashMap OOC_HashMap;
typedef struct OOC_HashMapClass OOC_HashMapClass;

struct OOC_HashMap {
    OOC_AbstractMap object;
    void* buckets;
    size_t size;
    float loadFactor;
};

struct OOC_HashMapClass {
    OOC_AbstractMapClass class;
};

#endif
