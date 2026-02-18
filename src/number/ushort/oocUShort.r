#ifndef OOC_USHORT_R_
#define OOC_USHORT_R_

#include "oocNumber.r"

typedef struct OOC_UShort OOC_UShort;
typedef struct OOC_UShortClass OOC_UShortClass;

struct OOC_UShort {
    OOC_Number number;
    unsigned short value;
};

struct OOC_UShortClass {
    OOC_NumberClass class;
};

#endif
