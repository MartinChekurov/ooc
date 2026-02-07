#ifndef OOC_HASH_MAP_ENTRY_R_
#define OOC_HASH_MAP_ENTRY_R_

#include "oocObject.r"

typedef struct OOC_HashMapEntry OOC_HashMapEntry;
typedef struct OOC_HashMapEntryClass OOC_HashMapEntryClass;

struct OOC_HashMapEntry {
    OOC_Object object;
    void* key;
    void* value;
};

struct OOC_HashMapEntryClass {
    OOC_Class class;
};

#endif
