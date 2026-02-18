#ifndef OOC_ULONG_R_
#define OOC_ULONG_R_

#include "oocNumber.r"

typedef struct OOC_ULong OOC_ULong;
typedef struct OOC_ULongClass OOC_ULongClass;

struct OOC_ULong {
    OOC_Number number;
    unsigned long value;
};

struct OOC_ULongClass {
    OOC_NumberClass class;
};

#endif
