#include "oocInterface.h"
#include "oocInterface.r"
#include "oocObject.h"

static OOC_InterfaceClass* InterfaceClass;
static OOC_InterfaceClass InterfaceClassInstance;

static void* ooc_interfaceClassInit(void) {
    if (ooc_classNew(&InterfaceClassInstance,
                    "Interface",
                    (size_t)0,
                    sizeof(OOC_InterfaceClass),
                    NULL,
                    OOC_MODIFIER_ABSTRACT,
                    0) != OOC_ERROR_NONE) {
        return NULL;
    }
    return &InterfaceClassInstance;
}

void* ooc_interfaceClass() {
    if (!InterfaceClass) {
        InterfaceClass = ooc_interfaceClassInit();
    }
    return InterfaceClass;
}