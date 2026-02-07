#ifndef OOC_STRING_BUFFER_R_
#define OOC_STRING_BUFFER_R_

#include "oocObject.r"

typedef struct OOC_StringBuffer OOC_StringBuffer;
typedef struct OOC_StringBufferClass OOC_StringBufferClass;

struct OOC_StringBuffer {
    struct OOC_Object object;
    char* data;
    size_t length;
    size_t capacity;
};

struct OOC_StringBufferClass {
    struct OOC_Class class;
};

#endif