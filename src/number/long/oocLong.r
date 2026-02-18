#ifndef OOC_LONG_R_
#define OOC_LONG_R_

#include "oocNumber.r"

typedef struct OOC_Long OOC_Long;
typedef struct OOC_LongClass OOC_LongClass;

struct OOC_Long {
    OOC_Number number;
    long value;
};

struct OOC_LongClass {
    OOC_NumberClass class;
};

#endif
