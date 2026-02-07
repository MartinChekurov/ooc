#ifndef OOC_STRING_R_
#define OOC_STRING_R_

#include "oocObject.r"

typedef struct OOC_String OOC_String;
typedef struct OOC_StringClass OOC_StringClass;

struct OOC_String {
    struct OOC_Object object;
    void* stringBuffer;
};

struct OOC_StringClass {
    struct OOC_Class class;
};

#endif