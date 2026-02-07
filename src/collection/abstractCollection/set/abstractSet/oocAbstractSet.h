/**
* @file oocAbstractSet.h
 * @brief Abstract base class for unordered Set implementations in the OOC framework
 *
 * This header defines the AbstractSet class, which extends AbstractCollection
 * with overrides for equals, hashCode, and toString to provide proper unordered
 * set semantics:
 * - equals: true if both sets contain the same elements (order-insensitive)
 * - hashCode: commutative sum of element hashes (order-insensitive)
 * - toString: uses curly braces "{elem1, elem2}" instead of square brackets
 *
 * Suitable for HashSet. LinkedHashSet should inherit directly from AbstractCollection
 * to preserve insertion-order semantics.
 *
 * This class is abstract and cannot be instantiated directly.
 */

#ifndef OOC_ABSTRACT_SET_H_
#define OOC_ABSTRACT_SET_H_

/**
 * @brief Returns the AbstractSet class descriptor
 * @return Pointer to the AbstractSet class object
 */
void* ooc_abstractSetClass();

#endif /* OOC_ABSTRACT_SET_H_ */