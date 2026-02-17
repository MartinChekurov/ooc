#include "oocObject.h"
#include "oocObject.r"
#include "oocGC.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*voidf)(void);

static OOC_Class* ObjectClass;
static OOC_Class ObjectClassInstance;

int ooc_classCountAbstractMethods(const void* class) {
    if (!class) {
        return 0;
    }
    const OOC_Class* c = class;
    int count = 0;
    size_t start = offsetof(OOC_Class, ctor);
    size_t end = ooc_classSelfSize(class);
    for (size_t off = start; off < end; off += sizeof(voidf)) {
        voidf* methodPtr = (voidf*)((char*)c + off);
        if (*methodPtr == NULL) {
            count++;
        }
    }
    return count;
}

OOC_Error ooc_classVerifyMethods(const void* class, size_t start, size_t end) {
    if (!class) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    for (size_t off = start; off < end; off += sizeof(voidf)) {
        voidf* methodPtr = (voidf*)((char*)class + off);
        if (*methodPtr == NULL) {
            return OOC_ERROR_NOT_IMPLEMENTED;
        }
    }
    return OOC_ERROR_NONE;
}

OOC_Error ooc_classOverrideMethods(void* class, va_list* args) {
    if (!class || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    int offset;
    while ((offset = va_arg(*args, int))) {
        voidf method = va_arg(*args, voidf);
        *(voidf*)((char*)class + offset) = method;
    }
    return OOC_ERROR_NONE;
}

OOC_Error ooc_classOverrideMethodsv(void* class, ...) {
    if (!class) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    va_list args;
    va_start(args, class);
    OOC_Error error = ooc_classOverrideMethods(class, &args);
    va_end(args);
    return error;
}

OOC_Error ooc_verifyMethods(void* self, size_t start, size_t end) {
    return ooc_classVerifyMethods(ooc_classOf(self), start, end);
}

OOC_Error ooc_overrideMethods(void* self, va_list* args) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    return ooc_classOverrideMethods(((OOC_Object*)self)->class, args);
}

OOC_Error ooc_overrideMethodsv(void* self, ...) {
    if (!self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    va_list args;
    va_start(args, self);
    OOC_Error error = ooc_overrideMethods(self, &args);
    va_end(args);
    return error;
}

static OOC_Error ooc_objectConstructor(void* self, va_list* args) {
    return OOC_ERROR_NONE;
}

static OOC_Error ooc_objectDestructor(void* self) {
    return OOC_ERROR_NONE;
}

static char* ooc_objectToString(const void* self) {
    if (!self) {
        return NULL;
    }
    const char* className = ooc_getClassName(self);
    size_t len = snprintf(NULL, 0, "%s@%p", className ? className : "Object", self);
    char* result = malloc(len + 1);
    if (!result) {
        return NULL;
    }
    snprintf(result, len + 1, "%s@%p", className ? className : "Object", self);
    return result;
}

static bool ooc_objectEquals(const void* self, const void* other) {
    return self == other;
}

static size_t ooc_objectHash(const void* self) {
    return self ? (size_t)self : 0;
}

static int ooc_objectCompare(const void* self, const void* other) {
    if (!self || !other) {
        return -1;
    }
    uintptr_t a = (uintptr_t)self;
    uintptr_t b = (uintptr_t)other;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

static void* ooc_objectClone(const void* self) {
    if (!self) {
        return NULL;
    }
    size_t size = ooc_sizeOf(self);
    if (!size) {
        return NULL;
    }
    OOC_Object* obj = calloc(1, size);
    if (!obj) {
        return NULL;
    }
    obj->class = (void*)ooc_classOf(self);
    ooc_gcRegister(obj);
    return obj;
}

static void ooc_objectGcMark(void* self, void* gc) {
    (void)self;
    (void)gc;
}

static void* ooc_objectClassInit(void) {
    memset(&ObjectClassInstance, 0, sizeof(ObjectClassInstance));

    ObjectClassInstance.name = "Object";
    ObjectClassInstance.super = NULL;

    ObjectClassInstance.size = sizeof(OOC_Object);
    ObjectClassInstance.selfSize = sizeof(OOC_Class);
    ObjectClassInstance.modifiers = OOC_MODIFIER_NONE;

    ObjectClassInstance.ctor = ooc_objectConstructor;
    ObjectClassInstance.dtor = ooc_objectDestructor;
    ObjectClassInstance.toString = ooc_objectToString;
    ObjectClassInstance.equals = ooc_objectEquals;
    ObjectClassInstance.hash = ooc_objectHash;
    ObjectClassInstance.compare = ooc_objectCompare;
    ObjectClassInstance.clone = ooc_objectClone;
    ObjectClassInstance.gc_mark = ooc_objectGcMark;

    return &ObjectClassInstance;
}

void* ooc_objectClass(void) {
    if (!ObjectClass) {
        ObjectClass = (OOC_Class*)ooc_objectClassInit();
    }
    return ObjectClass;
}

OOC_Error ooc_classNew(void* class, ...) {
    if (!class) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    va_list args;
    va_start(args, class);
    OOC_Error error = OOC_ERROR_NONE;
    OOC_Class* c = class;
    c->name = va_arg(args, char*);
    c->size = va_arg(args, size_t);
    c->selfSize = va_arg(args, size_t);
    c->super = va_arg(args, OOC_Class*);
    c->modifiers = (OOC_Modifier)va_arg(args, int);

    if (c->super) {
        if (ooc_classIsFinal(c->super)) {
            error = OOC_ERROR_FINAL_CLASS_INHERITANCE;
            goto end;
        }
        if ((c->modifiers & OOC_MODIFIER_ABSTRACT) && (c->modifiers & OOC_MODIFIER_FINAL)) {
            error = OOC_ERROR_INVALID_MODIFIER;
            goto end;
        }
        const size_t offset = offsetof(OOC_Class, ctor);
        memcpy((char*)c + offset,
               (char*)c->super + offset,
               ooc_classSelfSize(c->super) - offset);
    }
    c->interfaceCount = 0;
    c->interfaceImpls = NULL;
    error = ooc_classOverrideMethods(c, &args);
    if (error != OOC_ERROR_NONE) {
        goto end;
    }
    int abstractCount = ooc_classCountAbstractMethods(c);
    if (abstractCount && !(c->modifiers & OOC_MODIFIER_ABSTRACT)) {
        error = OOC_ERROR_INVALID_MODIFIER;
        goto end;
    }
end:
    va_end(args);
    return error;
}

OOC_Error ooc_classDestroy(void* class) {
    if (!class) {
        return OOC_ERROR_NONE;
    }
    OOC_Class* c = class;
    memset(c, 0, sizeof(*c));
    return OOC_ERROR_NONE;
}

const void* ooc_classSuper(const void* class) {
    if (!class) {
        return NULL;
    }
    return ((OOC_Class*)class)->super;
}

size_t ooc_classSelfSize(const void* class) {
    if (!class) {
        return 0;
    }
    return ((OOC_Class*)class)->selfSize;
}

size_t ooc_classSizeOf(const void* class) {
    if (!class) {
        return 0;
    }
    return (size_t)((OOC_Class*)class)->size;
}

const char* ooc_classGetName(const void* class) {
    if (!class) {
        return NULL;
    }
    return ((OOC_Class*)class)->name;
}

bool ooc_classIsAbstract(const void* class) {
    if (!class) {
        return false;
    }
    return ((OOC_Class*)class)->modifiers & OOC_MODIFIER_ABSTRACT;
}

bool ooc_classIsFinal(const void* class) {
    if (!class) {
        return false;
    }
    return ((OOC_Class*)class)->modifiers & OOC_MODIFIER_FINAL;
}

OOC_Error ooc_classCtor(const void* class, void* self, va_list* args) {
    if (!class || !self || !args) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_Class* c = class;
    if (!c->ctor) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return c->ctor(self, args);
}

OOC_Error ooc_classDtor(const void* class, void* self) {
    if (!class || !self) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    const OOC_Class* c = class;
    if (!c->dtor) {
        return OOC_ERROR_NOT_IMPLEMENTED;
    }
    return c->dtor(self);
}

char* ooc_classToString(const void* class, const void* self) {
    if (!class || !self) {
        return NULL;
    }
    const OOC_Class* c = class;
    return c->toString ? c->toString(self) : NULL;
}

bool ooc_classEquals(const void* class, const void* self, const void* other) {
    if (self == other) {
        return true;
    }
    if (!class || !self || !other) {
        return false;
    }
    const OOC_Class* c =class;
    return c->equals ? c->equals(self, other) : (self == other);
}

size_t ooc_classHashCode(const void* class, const void* self) {
    if (!class || !self) {
        return 0;
    }
    const OOC_Class* c = class;
    return c->hash ? c->hash(self) : 0;
}

int ooc_classCompare(const void* class, const void* self, const void* other) {
    if (!class || !self || !other) {
        return 1;
    }
    const OOC_Class* c = class;
    return c->compare ? c->compare(self, other) : 1;
}

void* ooc_classClone(const void* class, const void* self) {
    if (!class || !self) {
        return NULL;
    }
    const OOC_Class* c = class;
    return c->clone ? c->clone(self) : NULL;
}

static bool ooc_ifaceExtends(const OOC_Class* iface, const OOC_Class* target) {
    if (!iface || !target) {
        return false;
    }
    for (const OOC_Class* c = iface; c; c = c->super) {
        if (c == target) {
            return true;
        }
    }
    return false;
}

static const OOC_InterfaceImpl* ooc_classFindInterfaceImpl(const OOC_Class* class, const OOC_Class* targetIface) {
    if (!class || !targetIface) {
        return NULL;
    }
    for (const OOC_Class* c = class; c; c = c->super) {
        for (int i = 0; i < c->interfaceCount; i++) {
            const OOC_InterfaceImpl* impl = &c->interfaceImpls[i];
            if (impl->interfaceClass == targetIface) {
                return impl;
            }
        }
    }
    return NULL;
}

static bool ooc_classValidateInterfaces(const OOC_Class* class, const OOC_InterfaceImpl* impls, unsigned int count) {
    if (!class || !impls || !count) {
        return false;
    }
    for (unsigned int i = 0; i < count; i++) {
        const OOC_InterfaceImpl* impl = &impls[i];
        size_t vtableSize = ooc_classSelfSize(impl->interfaceClass) - sizeof(OOC_Class);
        if (ooc_classVerifyMethods(class,
                            impl->vtableOffset,
                            impl->vtableOffset + vtableSize) != OOC_ERROR_NONE) {
            return false;
        }
    }
    return true;
}

static bool ooc_classValidateParentInterfaces(const OOC_Class* c) {
    for (const OOC_Class* cur = c; cur; cur = cur->super) {
        if (!cur->interfaceImpls || cur->interfaceCount == 0) {
            continue;
        }
        for (unsigned int i = 0; i < cur->interfaceCount; i++) {
            const OOC_InterfaceImpl* impl = &cur->interfaceImpls[i];
            size_t vtableSize = ooc_classSelfSize(impl->interfaceClass) - sizeof(OOC_Class);
            if (ooc_classVerifyMethods(c,
                                       impl->vtableOffset,
                                       impl->vtableOffset + vtableSize) != OOC_ERROR_NONE) {
                return false;
            }
        }
    }
    return true;
}

OOC_Error ooc_classSetInterface(void* class, const void* impls, unsigned int count) {
    if (!class) {
        return OOC_ERROR_INVALID_ARGUMENT;
    }
    OOC_Class* c = class;
    c->interfaceImpls = impls;
    c->interfaceCount = count;
    return OOC_ERROR_NONE;
}

const void* ooc_classGetInterfaceVtable(const void* class, const void* interfaceType) {
    if (!class || !interfaceType) {
        return NULL;
    }
    const OOC_Class* c = class;
    const OOC_Class* targetIface = interfaceType;
    const OOC_InterfaceImpl* impl = ooc_classFindInterfaceImpl(c, targetIface);
    if (!impl) {
        return NULL;
    }
    return ((char*)c + impl->vtableOffset);
}

OOC_Error ooc_superCtor(const void* class, void* self, va_list* args) {
    return ooc_classCtor(ooc_classSuper(class), self, args);
}

OOC_Error ooc_superDtor(const void* class, void* self) {
    return ooc_classDtor(ooc_classSuper(class), self);
}

char* ooc_superToString(const void* class, const void* self) {
    return ooc_classToString(ooc_classSuper(class), self);
}

bool ooc_superEquals(const void* class, const void* self, const void* other) {
    return ooc_classEquals(ooc_classSuper(class), self, other);
}

size_t ooc_superHashCode(const void* class, const void* self) {
    return ooc_classHashCode(ooc_classSuper(class), self);
}

int ooc_superCompare(const void* class, const void* self, const void* other) {
    return ooc_classCompare(ooc_classSuper(class), self, other);
}

void* ooc_superClone(const void* class, const void* self) {
    return ooc_classClone(ooc_classSuper(class), self);
}

void* ooc_new(void* class, ...) {
    if (!class) {
        return NULL;
    }
    if (ooc_classIsAbstract(class)) {
        return NULL;
    }
    OOC_Class* class_ = class;
    OOC_Object* obj = calloc(1, ooc_classSizeOf(class_));
    if (!obj) {
        return NULL;
    }
    obj->class = class_;
    va_list args;
    va_start(args, class);
    OOC_Error error = ooc_classCtor(obj->class, obj, &args);
    va_end(args);
    if (error != OOC_ERROR_NONE) {
        ooc_destroy(obj);
        return NULL;
    }
    ooc_gcRegister(obj);
    return obj;
}

OOC_Error ooc_destroy(void* self) {
    if (!self) {
        return OOC_ERROR_NONE;
    }
    const OOC_Class* class = ooc_classOf(self);
    if (!class) {
        return OOC_ERROR_NO_CLASS;
    }
    ooc_gcUnregister(self);
    if (class->dtor) {
        OOC_Error error = class->dtor(self);
        if (error != OOC_ERROR_NONE) {
            return error;
        }
    }
    free(self);
    return OOC_ERROR_NONE;
}

const void* ooc_classOf(const void* self) {
    if (!self) {
        return NULL;
    }
    return ((const OOC_Object*)self)->class;
}

size_t ooc_sizeOf(const void* self) {
    return ooc_classSizeOf(ooc_classOf(self));
}

bool ooc_isInstanceOf(const void* self, const void* classOrIface) {
    if (!self || !classOrIface) {
        return false;
    }
    const OOC_Class* objClass = ooc_classOf(self);
    const OOC_Class* target  = classOrIface;
    for (const OOC_Class* c = objClass; c; c = c->super) {
        if (c == target) {
            return true;
        }
    }
    const OOC_InterfaceImpl* impl = ooc_classFindInterfaceImpl(objClass, target);
    return impl != NULL;
}

const char* ooc_getClassName(const void* self) {
    return ooc_classGetName(ooc_classOf(self));
}

bool ooc_isAbstract(const void* self) {
    return ooc_classIsAbstract(ooc_classOf(self));
}

bool ooc_isFinal(const void* self) {
    return ooc_classIsFinal(ooc_classOf(self));
}

char* ooc_toString(const void* self) {
    return ooc_classToString(ooc_classOf(self), self);
}

bool ooc_equals(const void* self, const void* other) {
    return ooc_classEquals(ooc_classOf(self), self, other);
}

size_t ooc_hashCode(const void* self) {
    return ooc_classHashCode(ooc_classOf(self), self);
}

int ooc_compare(const void* self, const void* other) {
    return ooc_classCompare(ooc_classOf(self), self, other);
}

void* ooc_clone(const void* self) {
    return ooc_classClone(ooc_classOf(self), self);
}

const void* ooc_getInterfaceVtable(const void* self, const void* interfaceType) {
    return ooc_classGetInterfaceVtable(ooc_classOf(self), interfaceType);
}

OOC_Error ooc_setInterface(void* self, const void* impls, unsigned int count) {
    return ooc_classSetInterface((void*)ooc_classOf(self), impls, count);
}
