#ifndef OOC_HASH_MAP_ENTRY_H_
#define OOC_HASH_MAP_ENTRY_H_

#include "oocError.h"

void* ooc_hashMapEntryClass();

void*       ooc_hashMapEntryGetKey  (void* self);
OOC_Error   ooc_hashMapEntrySetKey  (void* self, void* key);

void*       ooc_hashMapEntryGetValue(void* self);
OOC_Error   ooc_hashMapEntrySetValue(void* self, void* value);

#endif
