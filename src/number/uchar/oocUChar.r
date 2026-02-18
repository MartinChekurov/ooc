#ifndef OOC_UCHAR_R_
#define OOC_UCHAR_R_

#include "oocNumber.r"

typedef struct OOC_UChar OOC_UChar;
typedef struct OOC_UCharClass OOC_UCharClass;

struct OOC_UChar {
    OOC_Number number;
    unsigned char value;
};

struct OOC_UCharClass {
    OOC_NumberClass class;
};

#endif
