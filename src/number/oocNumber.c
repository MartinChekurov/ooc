#include "oocNumber.h"
#include "oocNumber.r"
#include "oocObject.h"

static OOC_NumberClass* NumberClass;
static OOC_NumberClass NumberClassInstance;

int ooc_numberIntValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_numberClass(), 0);
    OOC_NumberClass* c = (OOC_NumberClass*)ooc_classOf(self);
    return c->intValue(self);
}

unsigned int ooc_numberUIntValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_numberClass(), 0);
    OOC_NumberClass* c = (OOC_NumberClass*)ooc_classOf(self);
    return c->uintValue(self);
}

long ooc_numberLongValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_numberClass(), 0);
    OOC_NumberClass* c = (OOC_NumberClass*)ooc_classOf(self);
    return c->longValue(self);
}

unsigned long ooc_numberULongValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_numberClass(), 0);
    OOC_NumberClass* c = (OOC_NumberClass*)ooc_classOf(self);
    return c->ulongValue(self);
}

float ooc_numberFloatValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_numberClass(), 0);
    OOC_NumberClass* c = (OOC_NumberClass*)ooc_classOf(self);
    return c->floatValue(self);
}

double ooc_numberDoubleValue(const void* self) {
    OOC_TYPE_CHECK(self, ooc_numberClass(), 0);
    OOC_NumberClass* c = (OOC_NumberClass*)ooc_classOf(self);
    return c->doubleValue(self);
}

static void* ooc_numberClassInit(void) {
    if (ooc_classNew(&NumberClassInstance,
                    "Number",
                    sizeof(OOC_Number),
                    sizeof(OOC_NumberClass),
                    ooc_objectClass(),
                    OOC_MODIFIER_ABSTRACT,
                    0) != OOC_ERROR_NONE) {
        ooc_classDestroy(&NumberClassInstance);
        return NULL;
    }
    return &NumberClassInstance;
}

void* ooc_numberClass(void) {
    if (!NumberClass) {
        NumberClass = ooc_numberClassInit();
    }
    return NumberClass;
}
