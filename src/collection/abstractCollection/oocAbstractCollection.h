/**
* @file oocAbstractCollection.h
 * @brief Abstract base class for collections in the OOC framework
 *
 * This header defines the AbstractCollection class, which provides default
 * implementations for common Object methods (toString, equals, hashCode, compareTo, clone)
 * using the Iterable interface. It serves as a base for concrete collection classes
 * to avoid code duplication.
 *
 * This class is abstract and cannot be instantiated directly. Subclasses must
 * implement the required interfaces (e.g., Collection, Iterable) and may override
 * the provided methods if needed (e.g., for ordered vs. unordered semantics).
 */

#ifndef OOC_ABSTRACT_COLLECTION_H_
#define OOC_ABSTRACT_COLLECTION_H_

#include "oocError.h"
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Returns the AbstractCollection class descriptor
 * @return Pointer to the AbstractCollection class object
 */
void* ooc_abstractCollectionClass();

void* ooc_abstractCollectionGetIterator(void* self);
size_t ooc_abstractCollectionSize(void* self);
bool ooc_abstractCollectionIsEmpty(void* self);
bool ooc_abstractCollectionContains(void* self, void* object);
bool ooc_abstractCollectionContainsAll(void* self, void* other);
OOC_Error ooc_abstractCollectionAdd(void* self, void* element);
OOC_Error ooc_abstractCollectionRemove(void* self, void* object);
OOC_Error ooc_abstractCollectionClear(void* self);

#endif /* OOC_ABSTRACT_COLLECTION_H_ */
