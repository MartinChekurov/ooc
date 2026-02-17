#include "oocGC.h"
#include "oocObject.r"

#include <stdlib.h>
#include <string.h>

static OOC_Object* gc_head;
static void**      gc_roots;
static size_t      gc_root_count;
static size_t      gc_root_capacity;
static size_t      gc_object_count;

void ooc_gcInit(void) {
    gc_head = NULL;
    gc_roots = NULL;
    gc_root_count = 0;
    gc_root_capacity = 0;
    gc_object_count = 0;
}

void ooc_gcShutdown(void) {
    OOC_Object* obj = gc_head;
    while (obj) {
        OOC_Object* next = obj->gc_next;
        if (obj->class && obj->class->dtor) {
            obj->class->dtor(obj);
        }
        free(obj);
        obj = next;
    }
    gc_head = NULL;
    gc_object_count = 0;

    free(gc_roots);
    gc_roots = NULL;
    gc_root_count = 0;
    gc_root_capacity = 0;
}

void ooc_gcAddRoot(void** root) {
    if (!root) {
        return;
    }

    if (ooc_gcHasRoot(root)) {
        return;
    }

    if (gc_root_count == gc_root_capacity) {
        size_t new_cap = gc_root_capacity ? gc_root_capacity * 2 : 8;
        void** new_roots = realloc(gc_roots, new_cap * sizeof(void*));
        if (!new_roots) {
            return;
        }
        gc_roots = new_roots;
        gc_root_capacity = new_cap;
    }
    gc_roots[gc_root_count++] = root;
}

void ooc_gcRemoveRoot(void** root) {
    for (size_t i = 0; i < gc_root_count; i++) {
        if (gc_roots[i] == root) {
            gc_roots[i] = gc_roots[--gc_root_count];
            return;
        }
    }
}

void ooc_gcMark(void* obj) {
    if (!obj) {
        return;
    }
    OOC_Object* o = obj;
    if (o->gc_marked) {
        return;
    }
    o->gc_marked = true;
    if (o->class && o->class->gc_mark) {
        o->class->gc_mark(obj, NULL);
    }
}

void ooc_gcRun(void) {
    /* Mark phase */
    for (size_t i = 0; i < gc_root_count; i++) {
        void** root = gc_roots[i];
        if (*root) {
            ooc_gcMark(*root);
        }
    }

    /* Sweep phase */
    OOC_Object** prev = &gc_head;
    OOC_Object* obj = gc_head;
    while (obj) {
        OOC_Object* next = obj->gc_next;
        if (!obj->gc_marked) {
            *prev = next;
            if (obj->class && obj->class->dtor) {
                obj->class->dtor(obj);
            }
            free(obj);
            gc_object_count--;
        } else {
            obj->gc_marked = false;
            prev = &obj->gc_next;
        }
        obj = next;
    }
}

void ooc_gcRegister(void* obj) {
    if (!obj) {
        return;
    }
    OOC_Object* o = obj;
    o->gc_marked = false;
    o->gc_next = gc_head;
    gc_head = o;
    gc_object_count++;
}

void ooc_gcUnregister(void* obj) {
    if (!obj) {
        return;
    }
    OOC_Object* target = obj;
    OOC_Object** prev = &gc_head;
    OOC_Object* cur = gc_head;
    while (cur) {
        if (cur == target) {
            *prev = cur->gc_next;
            cur->gc_next = NULL;
            gc_object_count--;
            return;
        }
        prev = &cur->gc_next;
        cur = cur->gc_next;
    }
}

size_t ooc_gcObjectCount(void) {
    return gc_object_count;
}

size_t ooc_gcRootCount(void) {
    return gc_root_count;
}

bool ooc_gcHasRoot(void** root) {
    if (!root) {
        return false;
    }

    for (size_t i = 0; i < gc_root_count; i++) {
        if (gc_roots[i] == root) {
            return true;
        }
    }

    return false;
}
