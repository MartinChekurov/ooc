#ifndef OOC_LIST_R_
#define OOC_LIST_R_

#include "oocCollection.r"
#include "oocError.h"
#include <stddef.h>

typedef struct OOC_ListVtable OOC_ListVtable;
typedef struct OOC_ListClass OOC_ListClass;

struct OOC_ListVtable {
    OOC_CollectionVtable collectionVtable;
    void* (*getAt)(void* self, size_t index);
    OOC_Error (*setAt)(void* self, size_t index, void* element);
    OOC_Error (*insertAt)(void* self, size_t index, void* element);
    OOC_Error (*removeAt)(void* self, size_t index);
    int (*indexOf)(void* self, void* element);
    int (*lastIndexOf)(void* self, void* element);
};

struct OOC_ListClass {
    OOC_CollectionClass collectionClass;
    OOC_ListVtable vtable;
};

#define OOC_LIST_METHOD_GET_AT          OOC_METHOD(OOC_ListClass, vtable.getAt)
#define OOC_LIST_METHOD_SET_AT          OOC_METHOD(OOC_ListClass, vtable.setAt)
#define OOC_LIST_METHOD_INSERT_AT       OOC_METHOD(OOC_ListClass, vtable.insertAt)
#define OOC_LIST_METHOD_REMOVE_AT       OOC_METHOD(OOC_ListClass, vtable.removeAt)
#define OOC_LIST_METHOD_INDEX_OF        OOC_METHOD(OOC_ListClass, vtable.indexOf)
#define OOC_LIST_METHOD_LAST_INDEX_OF   OOC_METHOD(OOC_ListClass, vtable.lastIndexOf)

#endif
