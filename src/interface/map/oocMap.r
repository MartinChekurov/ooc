#ifndef OOC_MAP_R_
#define OOC_MAP_R_

#include "oocInterface.r"
#include "oocError.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct OOC_MapVtable OOC_MapVtable;
typedef struct OOC_MapClass OOC_MapClass;

struct OOC_MapVtable {
    size_t (*size)(void* self);
    bool (*isEmpty)(void* self);
    bool (*containsKey)(void* self, void* key);
    bool (*containsValue)(void* self, void* value);
    void* (*get)(void* self, void* key);
    OOC_Error (*put)(void* self, void* key, void* value);
    OOC_Error (*remove)(void* self, void* key);
    OOC_Error (*clear)(void* self);
    void* (*keySet)(void* self);
    void* (*values)(void* self);
};

struct OOC_MapClass {
    OOC_InterfaceClass interfaceClass;
    OOC_MapVtable vtable;
};

#define OOC_MAP_METHOD_SIZE             offsetof(OOC_MapClass, vtable.size)
#define OOC_MAP_METHOD_IS_EMPTY         offsetof(OOC_MapClass, vtable.isEmpty)
#define OOC_MAP_METHOD_CONTAINS_KEY     offsetof(OOC_MapClass, vtable.containsKey)
#define OOC_MAP_METHOD_CONTAINS_VALUE   offsetof(OOC_MapClass, vtable.containsValue)
#define OOC_MAP_METHOD_GET              offsetof(OOC_MapClass, vtable.get)
#define OOC_MAP_METHOD_PUT              offsetof(OOC_MapClass, vtable.put)
#define OOC_MAP_METHOD_REMOVE           offsetof(OOC_MapClass, vtable.remove)
#define OOC_MAP_METHOD_CLEAR            offsetof(OOC_MapClass, vtable.clear)
#define OOC_MAP_METHOD_KEY_SET          offsetof(OOC_MapClass, vtable.keySet)
#define OOC_MAP_METHOD_VALUES           offsetof(OOC_MapClass, vtable.values)

#endif
