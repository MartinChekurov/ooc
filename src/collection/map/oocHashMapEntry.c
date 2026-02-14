#include "oocHashMapEntry.h"
#include "oocHashMapEntry.r"
#include "oocObject.h"
#include "oocObject.r"
#include <stdlib.h>

static OOC_HashMapEntryClass* HashMapEntryClass;
static OOC_HashMapEntryClass HashMapEntryClassInstance;

void* ooc_hashMapEntryGetKey(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapEntryClass(), NULL);
    OOC_HashMapEntry* entry = self;
    return entry->key;
}

OOC_Error ooc_hashMapEntrySetKey(void* self, void* key) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapEntryClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMapEntry* entry = self;
    if (entry->key != key) {
        ooc_destroy(entry->key);
    }
    entry->key = key;
    return OOC_ERROR_NONE;
}

void* ooc_hashMapEntryGetValue(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapEntryClass(), NULL);
    OOC_HashMapEntry* entry = self;
    return entry->value;
}

OOC_Error ooc_hashMapEntrySetValue(void* self, void* value) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapEntryClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMapEntry* entry = self;
    if (entry->value != value) {
        ooc_destroy(entry->value);
    }
    entry->value = value;
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_hashMapEntryCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapEntryClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMapEntry* entry = self;
    OOC_Error error = ooc_superCtor(entry, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    entry->key = va_arg(*args, void*);
    entry->value = va_arg(*args, void*);
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_hashMapEntryDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashMapEntryClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashMapEntry* entry = self;
    if (entry->key) {
        ooc_destroy(entry->key);
        entry->key = NULL;
    }
    if (entry->value) {
        ooc_destroy(entry->value);
        entry->value = NULL;
    }
    return ooc_superDtor(entry);
}

static void* ooc_hashMapEntryClassInit(void) {
    if (ooc_classNew(&HashMapEntryClassInstance,
                    "HashMapEntry",
                    sizeof(OOC_HashMapEntry),
                    sizeof(OOC_HashMapEntryClass),
                    ooc_objectClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_hashMapEntryCtor,
                    OOC_METHOD_DTOR, ooc_hashMapEntryDtor,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &HashMapEntryClassInstance;
}

void* ooc_hashMapEntryClass(void) {
    if (!HashMapEntryClass) {
        HashMapEntryClass = ooc_hashMapEntryClassInit();
    }
    return HashMapEntryClass;
}
