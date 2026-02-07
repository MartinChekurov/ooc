#ifndef OOC_ABSTRACT_COLLECTION_R_
#define OOC_ABSTRACT_COLLECTION_R_

#include "oocObject.r"

typedef struct OOC_AbstractCollection OOC_AbstractCollection;
typedef struct OOC_AbstractCollectionClass OOC_AbstractCollectionClass;

struct OOC_AbstractCollection {
    OOC_Object object;
    size_t size;
};

struct OOC_AbstractCollectionClass {
    OOC_Class class;
};

#endif /* OOC_ABSTRACT_COLLECTION_R_ */