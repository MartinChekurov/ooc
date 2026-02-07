#ifndef OOC_LINKED_HASH_MAP_R_
#define OOC_LINKED_HASH_MAP_R_

#include "oocHashMap.r"

typedef struct OOC_LinkedHashMap OOC_LinkedHashMap;
typedef struct OOC_LinkedHashMapClass OOC_LinkedHashMapClass;

struct OOC_LinkedHashMap {
    OOC_HashMap hashMap;
    void* insertionOrder;
};

struct OOC_LinkedHashMapClass {
    OOC_HashMapClass class;
};

#endif
