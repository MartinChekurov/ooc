#include "oocSet.h"
#include "oocSet.r"
#include "oocCollection.h"
#include "oocObject.r"
#include "oocIterable.h"
#include <stddef.h>

static OOC_SetClass* SetClass;
static OOC_SetClass SetClassInstance;

static void* ooc_setClassInit(void) {
    if (ooc_classNew(&SetClassInstance,
                    "Set",
                    (size_t)0,
                    sizeof(OOC_SetClass),
                    ooc_collectionClass(),
                    OOC_MODIFIER_ABSTRACT,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &SetClassInstance;
}

void* ooc_setClass() {
    if (!SetClass) {
        SetClass = ooc_setClassInit();
    }
    return SetClass;
}

void* ooc_setGetIterator(void* self) {
    return ooc_iterableGetIterator(self);
}

size_t ooc_setSize(void* self) {
    return ooc_collectionSize(self);
}

bool ooc_setIsEmpty(void* self) {
    return ooc_collectionIsEmpty(self);
}

bool ooc_setContains(void* self, void* element) {
    return ooc_collectionContains(self, element);
}

bool ooc_setContainsAll(void* self, void* other) {
    return ooc_collectionContainsAll(self, other);
}

OOC_Error ooc_setAdd(void* self, void* element) {
    return ooc_collectionAdd(self, element);
}

OOC_Error ooc_setRemove(void* self, void* element) {
    return ooc_collectionRemove(self, element);
}

OOC_Error ooc_setClear(void* self) {
    return ooc_collectionClear(self);
}
