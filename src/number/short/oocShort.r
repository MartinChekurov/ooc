#ifndef OOC_SHORT_R_
#define OOC_SHORT_R_

#include "oocNumber.r"

typedef struct OOC_Short OOC_Short;
typedef struct OOC_ShortClass OOC_ShortClass;

struct OOC_Short {
    OOC_Number number;
    short value;
};

struct OOC_ShortClass {
    OOC_NumberClass class;
};

#endif
