#ifndef OOC_GC_H_
#define OOC_GC_H_

#include <stddef.h>

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
 * Roots define the "entry points" the GC starts from during marking.
 * In practice you usually root long-lived top-level references such as:
 * - application/service context objects
 * - current request/session context
 * - global caches or registries
 *
 * The input is the address of a pointer variable (`void**`), not the object.
 * This allows the GC to read the current pointer value when a collection runs.
 */
void ooc_gcAddRoot(void** root);

/**
 * @brief Unregisters a previously added root slot.
 */
void ooc_gcRemoveRoot(void** root);

/**
 * @brief Runs one full mark-and-sweep collection.
 *
 * Typical usage is to run this at safe points (end of request/frame/task).
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

/**
 * @brief Internal registration hook called by object allocation paths.
 */
void ooc_gcRegister(void* obj);

/**
 * @brief Internal unregistration hook called by explicit destroy paths.
 */
void ooc_gcUnregister(void* obj);

/**
 * @brief Convenience macro: root a pointer variable.
 *
 * Example:
 *   void* app = ooc_new(ooc_hashMapClass(), (size_t)0);
 *   OOC_GC_ROOT(app);
 */
#define OOC_GC_ROOT(var)   ooc_gcAddRoot((void**)&(var))

/**
 * @brief Convenience macro: unroot a pointer variable.
 */
#define OOC_GC_UNROOT(var) ooc_gcRemoveRoot((void**)&(var))


/**
 * @brief Creates a new object and roots the destination variable.
 *
 * Example:
 *   void* app = NULL;
 *   OOC_NEW_ROOTED(app, ooc_hashMapClass(), (size_t)0);
 */
#define OOC_NEW_ROOTED(var, class, ...)               do {                                                   (var) = ooc_new((class), ##__VA_ARGS__);          OOC_GC_ROOT(var);                             } while (0)

#if OOC_GC_HAS_SCOPED_ROOTS
/**
 * @brief GCC/Clang helper: creates a new object and auto-unroots on scope exit.
 */
#define OOC_NEW_SCOPED(var, class, ...)               (var) = ooc_new((class), ##__VA_ARGS__);           OOC_GC_SCOPED_ROOT(var)
#endif

#if OOC_GC_HAS_SCOPED_ROOTS
/**
 * @brief Cleanup callback used by `OOC_GC_SCOPED_ROOT`.
 *
 * It receives the address of a local variable that stores the root slot.
 */
static inline void ooc_gcCleanupRoot(void*** root_slot) {
    if (!root_slot || !*root_slot) {
        return;
    }
    ooc_gcRemoveRoot(*root_slot);
}

#define OOC_GC_CAT_(a, b) a##b
#define OOC_GC_CAT(a, b) OOC_GC_CAT_(a, b)

/**
 * @brief GCC/Clang scope-based root registration.
 *
 * Registers `var` as a root immediately, and automatically unregisters it when
 * the current scope exits (normal return, early return, or goto out of scope).
 *
 * Example:
 *   void* request = ooc_new(ooc_hashMapClass(), (size_t)0);
 *   OOC_GC_SCOPED_ROOT(request);
 */
#define OOC_GC_SCOPED_ROOT(var)                                                    \
    void** __attribute__((cleanup(ooc_gcCleanupRoot))) OOC_GC_CAT(_ooc_gc_root_,  \
                                                                   __LINE__) =     \
        (void**)&(var);                                                            \
    ooc_gcAddRoot(OOC_GC_CAT(_ooc_gc_root_, __LINE__))
#endif

#endif
