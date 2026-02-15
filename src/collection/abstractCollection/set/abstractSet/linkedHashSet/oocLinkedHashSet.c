#include "oocLinkedHashSet.h"
#include "oocLinkedHashSet.r"
#include "oocLinkedHashMap.h"
#include "oocObject.h"
#include "oocObject.r"
#include "oocHashSet.h"
#include <stdlib.h>

static OOC_LinkedHashSetClass* LinkedHashSetClass;
static OOC_LinkedHashSetClass LinkedHashSetClassInstance;

static OOC_Error ooc_linkedHashSetCtor(void* self, va_list* args) {
    if (!self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_TYPE_CHECK(self, ooc_linkedHashSetClass(), OOC_ERROR_INVALID_OBJECT);
    OOC_LinkedHashSet* set = self;
    OOC_Error error = ooc_superCtor(ooc_linkedHashSetClass(), set, args);
    if (error != OOC_ERROR_NONE) {
        return error;
    }
    /* Super ctor created a HashMap; replace it with a LinkedHashMap */
    ooc_destroy(set->object.map);
    set->object.map = ooc_new(ooc_linkedHashMapClass(), (size_t)0);
    if (!set->object.map) {
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
