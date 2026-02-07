#ifndef OOC_QUEUE_R_
#define OOC_QUEUE_R_

#include "oocCollection.r"
#include "oocError.h"
#include <stdbool.h>

typedef struct OOC_QueueVtable OOC_QueueVtable;
typedef struct OOC_QueueClass OOC_QueueClass;

struct OOC_QueueVtable {
    OOC_CollectionVtable collectionVtable;
    OOC_Error (*push)(void* self, void* element);
    void* (*pop)(void* self);
    void* (*peek)(void* self);
};

struct OOC_QueueClass {
    OOC_CollectionClass collectionClass;
    OOC_QueueVtable vtable;
};

#define OOC_QUEUE_METHOD_PUSH    OOC_METHOD(OOC_QueueClass, vtable.push)
#define OOC_QUEUE_METHOD_POP     OOC_METHOD(OOC_QueueClass, vtable.pop)
#define OOC_QUEUE_METHOD_PEEK    OOC_METHOD(OOC_QueueClass, vtable.peek)

#endif
