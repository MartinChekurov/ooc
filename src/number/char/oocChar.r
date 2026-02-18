#ifndef OOC_CHAR_R_
#define OOC_CHAR_R_

#include "oocNumber.r"

typedef struct OOC_Char OOC_Char;
typedef struct OOC_CharClass OOC_CharClass;

struct OOC_Char {
    OOC_Number number;
    char value;
};

struct OOC_CharClass {
    OOC_NumberClass class;
};

#endif
