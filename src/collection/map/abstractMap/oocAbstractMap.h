/**
 * @file oocAbstractMap.h
 * @brief Abstract base class for Map implementations in the OOC framework
 *
 * This header defines the AbstractMap class that provides a skeletal
 * implementation of the Map interface, to minimize the effort required
 * to implement this interface. This class is abstract and cannot be
 * instantiated directly.
 */

#ifndef OOC_ABSTRACT_MAP_H_
#define OOC_ABSTRACT_MAP_H_

/**
 * @brief Returns the AbstractMap class descriptor
 * @return Pointer to the AbstractMap class object
 */
void* ooc_abstractMapClass(void);

#endif
