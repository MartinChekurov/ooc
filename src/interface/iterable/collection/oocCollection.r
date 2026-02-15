#ifndef OOC_COLLECTION_R_
#define OOC_COLLECTION_R_

#include "oocIterable.r"
#include "oocError.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct OOC_CollectionClass OOC_CollectionClass;
typedef struct OOC_CollectionVtable OOC_CollectionVtable;

struct OOC_CollectionVtable {
    size_t (*size)(void* self);
    bool (*isEmpty)(void* self);
    bool (*contains)(void* self, void* element);
    bool (*containsAll)(void* self, void* other);
    OOC_Error (*add)(void* self, void* element);
    OOC_Error (*remove)(void* self, void* element);
    OOC_Error (*clear)(void* self);
};

struct OOC_CollectionClass {
    OOC_IterableClass iterableClass;
    OOC_CollectionVtable vtable;
};

#define OOC_COLLECTION_METHOD_SIZE              OOC_METHOD(OOC_CollectionClass, vtable.size)
#define OOC_COLLECTION_METHOD_IS_EMPTY          OOC_METHOD(OOC_CollectionClass, vtable.isEmpty)
#define OOC_COLLECTION_METHOD_CONTAINS          OOC_METHOD(OOC_CollectionClass, vtable.contains)
#define OOC_COLLECTION_METHOD_CONTAINS_ALL      OOC_METHOD(OOC_CollectionClass, vtable.containsAll)
#define OOC_COLLECTION_METHOD_ADD               OOC_METHOD(OOC_CollectionClass, vtable.add)
#define OOC_COLLECTION_METHOD_REMOVE            OOC_METHOD(OOC_CollectionClass, vtable.remove)
#define OOC_COLLECTION_METHOD_CLEAR             OOC_METHOD(OOC_CollectionClass, vtable.clear)

#endif
