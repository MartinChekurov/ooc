#ifndef OOC_DOUBLE_R_
#define OOC_DOUBLE_R_

#include "oocNumber.r"

typedef struct OOC_Double OOC_Double;
typedef struct OOC_DoubleClass OOC_DoubleClass;

struct OOC_Double {
    OOC_Number number;
    double value;
};

struct OOC_DoubleClass {
    OOC_NumberClass class;
};

#endif
