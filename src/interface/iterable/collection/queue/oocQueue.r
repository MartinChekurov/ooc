#ifndef OOC_QUEUE_R_
#define OOC_QUEUE_R_

#include "oocCollection.r"
#include "oocError.h"
#include <stdbool.h>

typedef struct OOC_QueueVtable OOC_QueueVtable;
typedef struct OOC_QueueClass OOC_QueueClass;

struct OOC_QueueVtable {
    OOC_Error (*offer)(void* self, void* element);
    void* (*poll)(void* self);
    void* (*peek)(void* self);
};

struct OOC_QueueClass {
    OOC_CollectionClass collectionClass;
    OOC_QueueVtable vtable;
};

#define OOC_QUEUE_METHOD_OFFER  OOC_METHOD(OOC_QueueClass, offer)
#define OOC_QUEUE_METHOD_POLL   OOC_METHOD(OOC_QueueClass, poll)
#define OOC_QUEUE_METHOD_PEEK   OOC_METHOD(OOC_QueueClass, peek)

#endif
