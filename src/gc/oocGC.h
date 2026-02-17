#ifndef OOC_GC_H_
#define OOC_GC_H_

#include <stddef.h>
#include "oocObject.h"

#if defined(__GNUC__) || defined(__clang__)
#define OOC_GC_HAS_SCOPED_ROOTS 1
#else
#define OOC_GC_HAS_SCOPED_ROOTS 0
#endif

/**
 * @brief Initializes the global GC runtime.
 *
 * Call once during application startup.
 */
void ooc_gcInit(void);

/**
 * @brief Frees all remaining tracked objects and GC internals.
 *
 * Call once during application shutdown.
 */
void ooc_gcShutdown(void);

/**
 * @brief Registers a root slot (address of a pointer variable).
 *
 * IMPORTANT: a root slot must outlive every GC cycle where that root is used.
 * Do not keep a root registered for a local variable after the function returns.
 */
void ooc_gcAddRoot(void** root);

/**
 * @brief Unregisters a previously added root slot.
 */
void ooc_gcRemoveRoot(void** root);

/**
 * @brief Runs one full mark-and-sweep collection.
 */
void ooc_gcRun(void);

/**
 * @brief Marks one object and recursively marks children via class->gc_mark.
 */
void ooc_gcMark(void* obj);

/**
 * @brief Returns the number of currently tracked objects.
 */
size_t ooc_gcObjectCount(void);

void ooc_gcRegister(void* obj);
void ooc_gcUnregister(void* obj);

#define OOC_GC_ROOT(var)   ooc_gcAddRoot((void**)&(var))
#define OOC_GC_UNROOT(var) ooc_gcRemoveRoot((void**)&(var))

/**
 * @brief Move root registration from one slot to another.
 *
 * Useful when a rooted local is returned/stored into a caller-owned slot.
 */
#define OOC_GC_MOVE_ROOT(from_slot, to_slot)  \
    do {                                       \
        ooc_gcRemoveRoot((void**)(from_slot)); \
        ooc_gcAddRoot((void**)(to_slot));      \
    } while (0)

/**
 * @brief Allocate into a caller-owned slot and root that slot.
 *
 * This is the safest pattern for "create here, use in another function" flows.
 */
#define OOC_GC_NEW_IN(slot, class, ...)            \
    do {                                            \
        *(slot) = ooc_new((class), ##__VA_ARGS__); \
        ooc_gcAddRoot((void**)(slot));             \
    } while (0)

/**
 * @brief Allocate and root a local/global pointer variable.
 */
#define OOC_NEW_ROOTED(var, class, ...)        \
    do {                                        \
        (var) = ooc_new((class), ##__VA_ARGS__); \
        OOC_GC_ROOT(var);                       \
    } while (0)

#if OOC_GC_HAS_SCOPED_ROOTS
static inline void ooc_gcCleanupRoot(void*** root_slot) {
    if (!root_slot || !*root_slot) {
        return;
    }
    ooc_gcRemoveRoot(*root_slot);
}

#define OOC_GC_CAT_(a, b) a##b
#define OOC_GC_CAT(a, b) OOC_GC_CAT_(a, b)

#define OOC_GC_SCOPED_ROOT(var)                                                    \
    void** __attribute__((cleanup(ooc_gcCleanupRoot))) OOC_GC_CAT(_ooc_gc_root_,  \
                                                                   __LINE__) =     \
        (void**)&(var);                                                            \
    ooc_gcAddRoot(OOC_GC_CAT(_ooc_gc_root_, __LINE__))

/**
 * @brief Allocate and auto-unroot at scope exit.
 */
#define OOC_NEW_SCOPED(var, class, ...)        \
    (var) = ooc_new((class), ##__VA_ARGS__);   \
    OOC_GC_SCOPED_ROOT(var)
#endif

#endif
