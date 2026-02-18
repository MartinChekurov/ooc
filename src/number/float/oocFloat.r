#ifndef OOC_FLOAT_R_
#define OOC_FLOAT_R_

#include "oocNumber.r"

typedef struct OOC_Float OOC_Float;
typedef struct OOC_FloatClass OOC_FloatClass;

struct OOC_Float {
    OOC_Number number;
    float value;
};

struct OOC_FloatClass {
    OOC_NumberClass class;
};

#endif
