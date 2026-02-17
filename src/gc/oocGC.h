#ifndef OOC_GC_H_
#define OOC_GC_H_

#include <stddef.h>

void   ooc_gcInit(void);
void   ooc_gcShutdown(void);
void   ooc_gcAddRoot(void** root);
void   ooc_gcRemoveRoot(void** root);
void   ooc_gcRun(void);
void   ooc_gcMark(void* obj);
size_t ooc_gcObjectCount(void);

void   ooc_gcRegister(void* obj);
void   ooc_gcUnregister(void* obj);

#endif
