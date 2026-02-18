#ifndef OOC_INTEGER_R_
#define OOC_INTEGER_R_

#include "oocNumber.r"

typedef struct OOC_Integer OOC_Integer;
typedef struct OOC_IntegerClass OOC_IntegerClass;

struct OOC_Integer {
    OOC_Number number;
    int value;
};

struct OOC_IntegerClass {
    OOC_NumberClass class;
};

#endif
