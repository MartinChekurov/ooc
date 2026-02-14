#include "oocHashSet.h"
#include "oocHashSet.r"
#include "oocHashMap.h"
#include "oocAbstractCollection.r"
#include "oocObject.h"
#include "oocObject.r"
#include "oocMap.h"
#include "oocAbstractSet.h"
#include <stdlib.h>

static OOC_HashSetClass* HashSetClass;
static OOC_HashSetClass HashSetClassInstance;

static void* DUMMY_VALUE = (void*)1;

static void* ooc_hashSetGetIterator(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetClass(), NULL);
    OOC_HashSet* set = self;
    return ooc_hashMapGetKeyIterator(set->map);
}

static size_t ooc_hashSetSize(void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetClass(), 0);
    OOC_HashSet* set = self;
    return ooc_mapSize(set->map);
}

static bool ooc_hashSetIsEmpty(void* self) {
    if (!self) {
        return true;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetClass(), true);
    OOC_HashSet* set = self;
    return ooc_mapIsEmpty(set->map);
}

static bool ooc_hashSetContains(void* self, void* element) {
    if (!self) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetClass(), false);
    OOC_HashSet* set = self;
    return ooc_mapContainsKey(set->map, element);
}

static OOC_Error ooc_hashSetAdd(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashSet* set = self;
    return ooc_mapPut(set->map, element, DUMMY_VALUE);
}

static OOC_Error ooc_hashSetRemove(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashSet* set = self;
    return ooc_mapRemove(set->map, element);
}

static OOC_Error ooc_hashSetClear(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashSet* set = self;
    return ooc_mapClear(set->map);
}

static OOC_Error ooc_hashSetCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashSet* set = self;
    OOC_Error error = ooc_superCtor(set, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    size_t initialCapacity = va_arg(*args, size_t);
    set->map = ooc_new(ooc_hashMapClass(), initialCapacity);
    if (!set->map) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_hashSetDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashSet* set = self;
    ooc_destroy(set->map);
    set->map = NULL;
    return ooc_superDtor(set);
}

static void* ooc_hashSetClassInit(void) {
    if (ooc_classNew(&HashSetClassInstance,
                    "HashSet",
                    sizeof(OOC_HashSet),
                    sizeof(OOC_HashSetClass),
                    ooc_abstractSetClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_hashSetCtor,
                    OOC_METHOD_DTOR, ooc_hashSetDtor,
                    OOC_ABSTRACT_COLLECTION_METHOD_ITERATOR, ooc_hashSetGetIterator,
                    OOC_ABSTRACT_COLLECTION_METHOD_SIZE, ooc_hashSetSize,
                    OOC_ABSTRACT_COLLECTION_METHOD_IS_EMPTY, ooc_hashSetIsEmpty,
                    OOC_ABSTRACT_COLLECTION_METHOD_CONTAINS, ooc_hashSetContains,
                    OOC_ABSTRACT_COLLECTION_METHOD_ADD, ooc_hashSetAdd,
                    OOC_ABSTRACT_COLLECTION_METHOD_REMOVE, ooc_hashSetRemove,
                    OOC_ABSTRACT_COLLECTION_METHOD_CLEAR, ooc_hashSetClear,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &HashSetClassInstance;
}

void* ooc_hashSetClass() {
    if (!HashSetClass) {
        HashSetClass = ooc_hashSetClassInit();
    }
    return HashSetClass;
}
