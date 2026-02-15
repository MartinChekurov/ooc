#ifndef OOC_DEQUE_R_
#define OOC_DEQUE_R_

#include "oocQueue.r"
#include "oocError.h"

typedef struct OOC_DequeVtable OOC_DequeVtable;
typedef struct OOC_DequeClass OOC_DequeClass;

struct OOC_DequeVtable {
    OOC_Error (*addFirst)(void* self, void* element);
    OOC_Error (*addLast)(void* self, void* element);
    void* (*removeFirst)(void* self);
    void* (*removeLast)(void* self);
    void* (*getFirst)(void* self);
    void* (*getLast)(void* self);
    OOC_Error (*push)(void* self, void* element);
    void* (*pop)(void* self);
};

struct OOC_DequeClass {
    OOC_QueueClass queueClass;
    OOC_DequeVtable vtable;
};

#define OOC_DEQUE_METHOD_ADD_FIRST      OOC_METHOD(OOC_DequeClass, vtable.addFirst)
#define OOC_DEQUE_METHOD_ADD_LAST       OOC_METHOD(OOC_DequeClass, vtable.addLast)
#define OOC_DEQUE_METHOD_REMOVE_FIRST   OOC_METHOD(OOC_DequeClass, vtable.removeFirst)
#define OOC_DEQUE_METHOD_REMOVE_LAST    OOC_METHOD(OOC_DequeClass, vtable.removeLast)
#define OOC_DEQUE_METHOD_GET_FIRST      OOC_METHOD(OOC_DequeClass, vtable.getFirst)
#define OOC_DEQUE_METHOD_GET_LAST       OOC_METHOD(OOC_DequeClass, vtable.getLast)
#define OOC_DEQUE_METHOD_GET_PUSH       OOC_METHOD(OOC_DequeClass, vtable.push)
#define OOC_DEQUE_METHOD_GET_POP        OOC_METHOD(OOC_DequeClass, vtable.pop)

#endif
