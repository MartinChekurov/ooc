#ifndef OOC_ABSTRACT_COLLECTION_R_
#define OOC_ABSTRACT_COLLECTION_R_

#include "oocObject.r"
#include "oocCollection.r"

typedef struct OOC_AbstractCollection OOC_AbstractCollection;
typedef struct OOC_AbstractCollectionClass OOC_AbstractCollectionClass;

struct OOC_AbstractCollection {
    OOC_Object object;
};

struct OOC_AbstractCollectionClass {
    OOC_Class class;
    OOC_CollectionVtable collectionVtable;
};

#define OOC_ABSTRACT_COLLECTION_METHOD_ITERATOR      offsetof(OOC_AbstractCollectionClass, collectionVtable.iterableVtable.iterator)
#define OOC_ABSTRACT_COLLECTION_METHOD_SIZE          offsetof(OOC_AbstractCollectionClass, collectionVtable.size)
#define OOC_ABSTRACT_COLLECTION_METHOD_IS_EMPTY      offsetof(OOC_AbstractCollectionClass, collectionVtable.isEmpty)
#define OOC_ABSTRACT_COLLECTION_METHOD_CONTAINS      offsetof(OOC_AbstractCollectionClass, collectionVtable.contains)
#define OOC_ABSTRACT_COLLECTION_METHOD_CONTAINS_ALL  offsetof(OOC_AbstractCollectionClass, collectionVtable.containsAll)
#define OOC_ABSTRACT_COLLECTION_METHOD_ADD           offsetof(OOC_AbstractCollectionClass, collectionVtable.add)
#define OOC_ABSTRACT_COLLECTION_METHOD_REMOVE        offsetof(OOC_AbstractCollectionClass, collectionVtable.remove)
#define OOC_ABSTRACT_COLLECTION_METHOD_CLEAR         offsetof(OOC_AbstractCollectionClass, collectionVtable.clear)

#endif /* OOC_ABSTRACT_COLLECTION_R_ */
