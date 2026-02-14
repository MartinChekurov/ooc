#include "oocLinkedHashSet.h"
#include "oocLinkedHashSet.r"
#include "oocLinkedHashMap.h"
#include "oocAbstractCollection.r"
#include "oocObject.h"
#include "oocObject.r"
#include "oocHashSet.h"
#include <stdlib.h>

static OOC_LinkedHashSetClass* LinkedHashSetClass;
static OOC_LinkedHashSetClass LinkedHashSetClassInstance;

static void* ooc_linkedHashSetGetIterator(void* self) {
    if (!self) {
        return NULL;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashSetClass(), NULL);
    OOC_LinkedHashSet* set = self;
    return ooc_linkedHashMapGetIterator(set->map);
}

static OOC_Error ooc_linkedHashSetCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashSetClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashSet* set = self;
    OOC_Error error = ooc_superCtor(set, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    size_t initialCapacity = va_arg(*args, size_t);
    set->map = ooc_new(ooc_linkedHashMapClass(), initialCapacity);
    if (!set->map) {
        return OOC_ERROR_OUT_OF_MEMORY;
    }
    return OOC_ERROR_NONE;
}

static void* ooc_linkedHashSetClassInit(void) {
    if (ooc_classNew(&LinkedHashSetClassInstance,
                    "LinkedHashSet",
                    sizeof(OOC_LinkedHashSet),
                    sizeof(OOC_LinkedHashSetClass),
                    ooc_hashSetClass(),
                    OOC_MODIFIER_NONE,
                    OOC_METHOD_CTOR, ooc_linkedHashSetCtor,
                    OOC_ABSTRACT_COLLECTION_METHOD_ITERATOR, ooc_linkedHashSetGetIterator,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &LinkedHashSetClassInstance;
}

void* ooc_linkedHashSetClass() {
    if (!LinkedHashSetClass) {
        LinkedHashSetClass = ooc_linkedHashSetClassInit();
    }
    return LinkedHashSetClass;
}
