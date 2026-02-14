
#include "oocMap.h"
#include "oocMap.r"
#include "oocInterface.h"
#include "oocObject.h"

static OOC_MapClass* MapClass;
static OOC_MapClass MapClassInstance;

static void* ooc_mapClassInit(void) {
    if (ooc_classNew(&MapClassInstance,
                    "Map",
                    (size_t)0,
                    sizeof(const OOC_MapClass),
                    ooc_interfaceClass(),
                    OOC_MODIFIER_ABSTRACT,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &MapClassInstance;
}

void* ooc_mapClass() {
    if (!MapClass) {
        MapClass = ooc_mapClassInit();
    }
    return MapClass;
}

size_t ooc_mapSize(void* self) {
    if (!self) {
        return 0;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable || !vtable->size) {
        return 0;
    }
    return vtable->size(self);
}

bool ooc_mapIsEmpty(void* self) {
    if (!self) {
        return true;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable || !vtable->isEmpty) {
        return true;
    }
    return vtable->isEmpty(self);
}

bool ooc_mapContainsKey(void* self, void* key) {
    if (!self) {
        return false;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable || !vtable->containsKey) {
        return false;
    }
    return vtable->containsKey(self, key);
}

bool ooc_mapContainsValue(void* self, void* value) {
    if (!self) {
        return false;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable || !vtable->containsValue) {
        return false;
    }
    return vtable->containsValue(self, value);
}

void* ooc_mapGet(void* self, void* key) {
    if (!self) {
        return NULL;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable || !vtable->get) {
        return NULL;
    }
    return vtable->get(self, key);
}

OOC_Error ooc_mapPut(void* self, void* key, void* value) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable || !vtable->put) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->put(self, key, value);
}

OOC_Error ooc_mapRemove(void* self, void* key) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable || !vtable->remove) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->remove(self, key);
}

OOC_Error ooc_mapClear(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable || !vtable->clear) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->clear(self);
}

void* ooc_mapKeySet(void* self) {
    if (!self) {
        return NULL;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable || !vtable->keySet) {
        return NULL;
    }
    return vtable->keySet(self);
}

void* ooc_mapValues(void* self) {
    if (!self) {
        return NULL;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable || !vtable->values) {
        return NULL;
    }
    return vtable->values(self);
}
