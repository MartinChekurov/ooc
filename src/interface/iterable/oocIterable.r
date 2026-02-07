#ifndef OOC_ITERABLE_R_
#define OOC_ITERABLE_R_

#include "oocInterface.r"
#include "oocError.h"
#include <stdbool.h>

typedef struct OOC_IterableClass OOC_IterableClass;
typedef struct OOC_IterableVtable OOC_IterableVtable;

struct OOC_IterableVtable {
    void* (*iterator)(void* self);
};

struct OOC_IterableClass {
    OOC_InterfaceClass super;
    OOC_IterableVtable vtable;
};

#define OOC_ITERABLE_METHOD_ITERATOR    OOC_METHOD(OOC_IterableClass, vtable.iterator)

#endif
