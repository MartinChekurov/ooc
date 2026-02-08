#ifndef OOC_ABSTRACT_LIST_R_
#define OOC_ABSTRACT_LIST_R_

#include "oocAbstractCollection.r"
#include "oocList.r"
#include <stddef.h>

typedef struct OOC_AbstractList OOC_AbstractList;
typedef struct OOC_AbstractListClass OOC_AbstractListClass;

struct OOC_AbstractList {
    OOC_AbstractCollection abstractCollection;
};

struct OOC_AbstractListClass {
    OOC_AbstractCollectionClass class;
    OOC_ListVtable listVtable;
};

#define OOC_ABSTRACT_LIST_METHOD_GET_AT               offsetof(OOC_AbstractListClass, listVtable.getAt)
#define OOC_ABSTRACT_LIST_METHOD_SET_AT               offsetof(OOC_AbstractListClass, listVtable.setAt)
#define OOC_ABSTRACT_LIST_METHOD_INSERT_AT            offsetof(OOC_AbstractListClass, listVtable.insertAt)
#define OOC_ABSTRACT_LIST_METHOD_REMOVE_AT            offsetof(OOC_AbstractListClass, listVtable.removeAt)
#define OOC_ABSTRACT_LIST_METHOD_INDEX_OF             offsetof(OOC_AbstractListClass, listVtable.indexOf)
#define OOC_ABSTRACT_LIST_METHOD_LAST_INDEX_OF        offsetof(OOC_AbstractListClass, listVtable.lastIndexOf)
#define OOC_ABSTRACT_LIST_METHOD_GET_LIST_ITERATOR    offsetof(OOC_AbstractListClass, listVtable.getListIterator)
#define OOC_ABSTRACT_LIST_METHOD_GET_LIST_ITERATOR_AT offsetof(OOC_AbstractListClass, listVtable.getListIteratorAt)
#
#endif /* OOC_ABSTRACT_LIST_R_ */
