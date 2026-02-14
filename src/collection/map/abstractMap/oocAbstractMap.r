#ifndef OOC_ABSTRACT_MAP_R_
#define OOC_ABSTRACT_MAP_R_

#include "oocObject.r"
#include "oocMap.r"

typedef struct OOC_AbstractMap OOC_AbstractMap;
typedef struct OOC_AbstractMapClass OOC_AbstractMapClass;

struct OOC_AbstractMap {
    OOC_Object object;
};

struct OOC_AbstractMapClass {
    OOC_Class class;
    OOC_MapVtable mapVtable;
};

#define OOC_ABSTRACT_MAP_METHOD_SIZE           offsetof(OOC_AbstractMapClass, mapVtable.size)
#define OOC_ABSTRACT_MAP_METHOD_IS_EMPTY       offsetof(OOC_AbstractMapClass, mapVtable.isEmpty)
#define OOC_ABSTRACT_MAP_METHOD_CONTAINS_KEY   offsetof(OOC_AbstractMapClass, mapVtable.containsKey)
#define OOC_ABSTRACT_MAP_METHOD_CONTAINS_VALUE offsetof(OOC_AbstractMapClass, mapVtable.containsValue)
#define OOC_ABSTRACT_MAP_METHOD_GET            offsetof(OOC_AbstractMapClass, mapVtable.get)
#define OOC_ABSTRACT_MAP_METHOD_PUT            offsetof(OOC_AbstractMapClass, mapVtable.put)
#define OOC_ABSTRACT_MAP_METHOD_REMOVE         offsetof(OOC_AbstractMapClass, mapVtable.remove)
#define OOC_ABSTRACT_MAP_METHOD_CLEAR          offsetof(OOC_AbstractMapClass, mapVtable.clear)
#define OOC_ABSTRACT_MAP_METHOD_KEY_SET        offsetof(OOC_AbstractMapClass, mapVtable.keySet)
#define OOC_ABSTRACT_MAP_METHOD_VALUES         offsetof(OOC_AbstractMapClass, mapVtable.values)
#define OOC_ABSTRACT_MAP_METHOD_GET_ITERATOR   offsetof(OOC_AbstractMapClass, mapVtable.getIterator)

#endif
