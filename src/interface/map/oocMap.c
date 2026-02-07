#include "oocMap.h"
#include "oocMap.r"
#include "oocInterface.h"
#include "oocInterface.r"
#include "oocIterable.h"
#include "oocObject.h"

static OOC_MapClass* MapClass;
static OOC_MapClass MapClassInstance;

static void* ooc_mapClassInit(void) {
    if (ooc_classNew(&MapClassInstance,
                    "Collection",
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
    if (!vtable) {
        return 0;
    }
    if (!vtable->size) {
        return 0;
    }
    return vtable->size(self);
}

bool ooc_mapIsEmpty(void* self) {
    if (!self) {
        return true;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable) {
        return true;
    }
    if (!vtable->isEmpty) {
        return true;
    }
    return vtable->isEmpty(self);
}

bool ooc_mapContainsKey(void* self, void* key) {
    if (!self || !key) {
        return false;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable) {
        return false;
    }
    if (!vtable->containsKey) {
        return false;
    }
    return vtable->containsKey(self, key);
}

bool ooc_mapContainsValue(void* self, void* value) {
    if (!self || !value) {
        return false;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable) {
        return false;
    }
    if (!vtable->containsValue) {
        return false;
    }
    return vtable->containsValue(self, value);
}

void* ooc_mapGet(void* self, void* key) {
    if (!self || !key) {
        return NULL;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable) {
        return NULL;
    }
    if (!vtable->get) {
        return NULL;
    }
    return vtable->get(self, key);
}

OOC_Error ooc_mapPut(void* self, void* key, void* value) {
    if (!self || !key || !value) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    if (!vtable->put) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->put(self, key, value);
}

OOC_Error ooc_mapRemove(void* self, void* key) {
    if (!self || !key) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    if (!vtable->remove) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->remove(self, key);
}

OOC_Error ooc_mapClear(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    if (!vtable->clear) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return vtable->clear(self);
}

void* ooc_mapKeySet(void* self) {
    if (!self) {
        return NULL;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable) {
        return NULL;
    }
    if (!vtable->keySet) {
        return NULL;
    }
    return vtable->keySet(self);
}

void* ooc_mapValues(void* self) {
    if (!self) {
        return NULL;
    }
    const OOC_MapVtable* vtable = ooc_getInterfaceVtable(self, ooc_mapClass());
    if (!vtable) {
        return NULL;
    }
    if (!vtable->values) {
        return NULL;
    }
    return vtable->values(self);
}