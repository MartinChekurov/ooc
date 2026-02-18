#ifndef OOC_UINTEGER_R_
#define OOC_UINTEGER_R_

#include "oocNumber.r"

typedef struct OOC_UInteger OOC_UInteger;
typedef struct OOC_UIntegerClass OOC_UIntegerClass;

struct OOC_UInteger {
    OOC_Number number;
    unsigned int value;
};

struct OOC_UIntegerClass {
    OOC_NumberClass class;
};

#endif
