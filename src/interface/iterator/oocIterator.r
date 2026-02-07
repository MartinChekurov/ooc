#ifndef OOC_ITERATOR_
#define OOC_ITERATOR_

#include "oocInterface.r"
#include "oocError.h"
#include <stdbool.h>

typedef struct OOC_IteratorVtable OOC_IteratorVtable;
typedef struct OOC_IteratorClass OOC_IteratorClass;

struct OOC_IteratorVtable {
    bool (*hasNext)(void* self);
    void* (*next)(void* self);
    OOC_Error (*remove)(void* self);
};

struct OOC_IteratorClass {
    OOC_InterfaceClass super;
    OOC_IteratorVtable vtable;
};

#define OOC_ITERATOR_METHOD_HAS_NEXT    OOC_METHOD(OOC_IteratorClass, vtable.hasNext)
#define OOC_ITERATOR_METHOD_NEXT        OOC_METHOD(OOC_IteratorClass, vtable.next)
#define OOC_ITERATOR_METHOD_REMOVE      OOC_METHOD(OOC_IteratorClass, vtable.remove)

#endif
