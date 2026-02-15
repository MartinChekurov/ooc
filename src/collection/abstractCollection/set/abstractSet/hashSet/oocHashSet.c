#include "oocHashSet.h"
#include "oocHashSet.r"
#include "oocHashMap.h"
#include "oocHashMapEntry.h"
#include "oocAbstractCollection.r"
#include "oocAbstractIterator.h"
#include "oocAbstractIterator.r"
#include "oocObject.h"
#include "oocObject.r"
#include "oocMap.h"
#include "oocIterator.h"
#include "oocAbstractSet.h"
#include <stdlib.h>

typedef struct OOC_HashSetIterator OOC_HashSetIterator;
typedef struct OOC_HashSetIteratorClass OOC_HashSetIteratorClass;

struct OOC_HashSetIterator {
    OOC_AbstractIterator object;
    void* mapIterator;
};

struct OOC_HashSetIteratorClass {
    OOC_AbstractIteratorClass class;
};

static OOC_HashSetClass* HashSetClass;
static OOC_HashSetClass HashSetClassInstance;

static OOC_HashSetIteratorClass* HashSetIteratorClass;
static OOC_HashSetIteratorClass HashSetIteratorClassInstance;

static void* DUMMY_VALUE = NULL;

void* ooc_hashSetIteratorClass(void);

static bool ooc_hashSetIteratorHasNext(void* self) {
    if (!self) {
        return false;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetIteratorClass(), false);
    OOC_HashSetIterator* iterator = self;
    return ooc_iteratorHasNext(iterator->mapIterator);
}

static void* ooc_hashSetIteratorNext(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetIteratorClass(), NULL);
    OOC_HashSetIterator* iterator = self;
    ooc_abstractIteratorNext(iterator);
    void* entry = ooc_iteratorNext(iterator->mapIterator);
    return ooc_hashMapEntryGetKey(entry);
}

static OOC_Error ooc_hashSetIteratorRemove(void* self) {
    if (!self) {
        return OOC_ERROR_NULL_POINTER;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashSetIterator* iterator = self;
    OOC_Error error = ooc_abstractIteratorRemove(iterator);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    return ooc_iteratorRemove(iterator->mapIterator);
}

static OOC_Error ooc_hashSetIteratorCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashSetIterator* iterator = self;
    OOC_Error error = ooc_superCtor(ooc_hashSetIteratorClass(), iterator, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    OOC_HashSet* set = va_arg(*args, OOC_HashSet*);
    if (!set) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    iterator->mapIterator = ooc_mapGetIterator(set->map);
    if (!iterator->mapIterator) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_hashSetIteratorDtor(void* self) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetIteratorClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashSetIterator* iterator = self;
    ooc_destroy(iterator->mapIterator);
    iterator->mapIterator = NULL;
    return ooc_superDtor(ooc_hashSetIteratorClass(), self);
}

static void* ooc_hashSetIteratorClassInit(void) {
    if (ooc_classNew(&HashSetIteratorClassInstance,
                    "HashSetIterator",
                    sizeof(OOC_HashSetIterator),
                    sizeof(OOC_HashSetIteratorClass),
                    ooc_abstractIteratorClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_hashSetIteratorCtor,
                    OOC_METHOD_DTOR, ooc_hashSetIteratorDtor,
                    OOC_ABSTRACT_ITERATOR_METHOD_HAS_NEXT, ooc_hashSetIteratorHasNext,
                    OOC_ABSTRACT_ITERATOR_METHOD_NEXT, ooc_hashSetIteratorNext,
                    OOC_ABSTRACT_ITERATOR_METHOD_REMOVE, ooc_hashSetIteratorRemove,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &HashSetIteratorClassInstance;
}

void* ooc_hashSetIteratorClass(void) {
    if (!HashSetIteratorClass) {
        HashSetIteratorClass = ooc_hashSetIteratorClassInit();
    }
    return HashSetIteratorClass;
}

static void* ooc_hashSetGetIterator(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetClass(), NULL);
    return ooc_new(ooc_hashSetIteratorClass(), self);
}

static size_t ooc_hashSetSize(void* self) {
    if (!self) {
        return 0;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetClass(), 0);
    OOC_HashSet* set = self;
    return ooc_mapSize(set->map);
}

static OOC_Error ooc_hashSetAdd(void* self, void* element) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashSet* set = self;
    return ooc_mapPut(set->map, element, DUMMY_VALUE);
}

static OOC_Error ooc_hashSetCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_hashSetClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_HashSet* set = self;
    OOC_Error error = ooc_superCtor(ooc_hashSetClass(), set, args);
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
    return ooc_superDtor(ooc_hashSetClass(), set);
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
                    OOC_ABSTRACT_COLLECTION_METHOD_ADD, ooc_hashSetAdd,
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
