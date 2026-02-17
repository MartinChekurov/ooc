#ifndef OOC_OBJECT_R_
#define OOC_OBJECT_R_

#include "oocError.h"
#include "oocModifiers.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct OOC_InterfaceImpl OOC_InterfaceImpl;
typedef struct OOC_Object OOC_Object;
typedef struct OOC_Class OOC_Class;

#define OOC_TYPE_CHECK(object, class, error)\
    do {\
        if (!ooc_isInstanceOf((object), (class))) {\
            return (error);\
        }\
    } while (0)


struct OOC_InterfaceImpl {
    const OOC_Class* interfaceClass;
    size_t vtableOffset;
};

struct OOC_Class {
    char* name;
    struct OOC_Class* super;

    size_t size;
    size_t selfSize;
    OOC_Modifier modifiers;

    unsigned int interfaceCount;
    const OOC_InterfaceImpl* interfaceImpls;

    OOC_Error (*ctor)(void* self, va_list* args);
    OOC_Error (*dtor)(void* self);
    char* (*toString)(const void* self);
    bool (*equals)(const void* self, const void* other);
    size_t (*hash)(const void* self);
    int (*compare)(const void* self, const void* other);
    void* (*clone)(const void* self);
    void (*gc_mark)(void* self, void* gc);
};

struct OOC_Object {
    OOC_Class* class;
    bool gc_marked;
    struct OOC_Object* gc_next;
};

OOC_Error   ooc_classNew       (void* class, ...);
OOC_Error   ooc_classDestroy   (void* class);

const void* ooc_classSuper                  (const void* class);
size_t      ooc_classSelfSize               (const void* class);
size_t      ooc_classSizeOf                 (const void* class);
const char* ooc_classGetName                (const void* class);
bool        ooc_classIsAbstract             (const void* class);
bool        ooc_classIsFinal                (const void* class);
OOC_Error   ooc_classCtor                   (const void* class, void* self, va_list* args);
OOC_Error   ooc_classDtor                   (const void* class, void* self);
char*       ooc_classToString               (const void* class, const void* self);
bool        ooc_classEquals                 (const void* class, const void* self, const void* other);
size_t      ooc_classHashCode               (const void* class, const void* self);
int         ooc_classCompare                (const void* class, const void* self, const void* other);
void*       ooc_classClone                  (const void* class, const void* self);

OOC_Error   ooc_classSetInterface           (void* class, const void* impls, unsigned int count);
const void* ooc_classGetInterfaceVtable     (const void* class, const void* interfaceType);

int         ooc_classCountAbstractMethods   (const void* class);
OOC_Error   ooc_classVerifyMethods          (const void* class, size_t start, size_t end);
OOC_Error   ooc_classOverrideMethods        (void* class, va_list* args);
OOC_Error   ooc_classOverrideMethodsv       (void* class, ...);

OOC_Error   ooc_superCtor           (const void* class, void* self, va_list* args);
OOC_Error   ooc_superDtor           (const void* class, void* self);
char*       ooc_superToString       (const void* class, const void* self);
bool        ooc_superEquals         (const void* class, const void* self, const void* other);
size_t      ooc_superHashCode       (const void* class, const void* self);
int         ooc_superCompare        (const void* class, const void* self, const void* other);
void*       ooc_superClone          (const void* class, const void* self);

#define OOC_METHOD(class, method)   offsetof(class, method)
#define OOC_METHOD_CTOR             OOC_METHOD(OOC_Class, ctor)
#define OOC_METHOD_DTOR             OOC_METHOD(OOC_Class, dtor)
#define OOC_METHOD_TO_STRING        OOC_METHOD(OOC_Class, toString)
#define OOC_METHOD_EQUALS           OOC_METHOD(OOC_Class, equals)
#define OOC_METHOD_HASH             OOC_METHOD(OOC_Class, hash)
#define OOC_METHOD_COMPARE          OOC_METHOD(OOC_Class, compare)
#define OOC_METHOD_CLONE            OOC_METHOD(OOC_Class, clone)
#define OOC_METHOD_GC_MARK          OOC_METHOD(OOC_Class, gc_mark)

#endif
