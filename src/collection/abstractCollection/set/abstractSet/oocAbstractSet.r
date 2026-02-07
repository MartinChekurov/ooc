#ifndef OOC_ABSTRACT_SET_R_
#define OOC_ABSTRACT_SET_R_

#include "oocAbstractCollection.r"

typedef struct OOC_AbstractSet OOC_AbstractSet;
typedef struct OOC_AbstractSetClass OOC_AbstractSetClass;

struct OOC_AbstractSet {
    OOC_AbstractCollection abstractCollection;
};

struct OOC_AbstractSetClass {
    OOC_AbstractCollectionClass class;
};

#endif /* OOC_ABSTRACT_SET_R_ */