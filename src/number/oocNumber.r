#ifndef OOC_NUMBER_R_
#define OOC_NUMBER_R_

#include "oocObject.r"

typedef struct OOC_Number OOC_Number;
typedef struct OOC_NumberClass OOC_NumberClass;

struct OOC_Number {
    OOC_Object object;
};

struct OOC_NumberClass {
    OOC_Class class;
    int            (*intValue)(const void* self);
    unsigned int   (*uintValue)(const void* self);
    long           (*longValue)(const void* self);
    unsigned long  (*ulongValue)(const void* self);
    float          (*floatValue)(const void* self);
    double         (*doubleValue)(const void* self);
};

#define OOC_NUMBER_METHOD_INT_VALUE    offsetof(OOC_NumberClass, intValue)
#define OOC_NUMBER_METHOD_UINT_VALUE   offsetof(OOC_NumberClass, uintValue)
#define OOC_NUMBER_METHOD_LONG_VALUE   offsetof(OOC_NumberClass, longValue)
#define OOC_NUMBER_METHOD_ULONG_VALUE  offsetof(OOC_NumberClass, ulongValue)
#define OOC_NUMBER_METHOD_FLOAT_VALUE  offsetof(OOC_NumberClass, floatValue)
#define OOC_NUMBER_METHOD_DOUBLE_VALUE offsetof(OOC_NumberClass, doubleValue)

#endif
