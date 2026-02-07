/**
* @file oocInterface.h
 * @brief Interface support for the OOC (Object-Oriented C) framework
 *
 * This header defines the basic interface infrastructure for the OOC framework.
 * Interfaces provide a way to define contracts that classes can implement,
 * similar to interfaces in object-oriented languages like Java.
 */

#ifndef OOC_INTERFACE_H_
#define OOC_INTERFACE_H_

#include "oocError.h"

/**
 * @brief Returns the Interface class descriptor
 * @return Pointer to the Interface class object
 *
 * This function returns the class descriptor for the base Interface class,
 * which serves as the root for all interface types in the OOC framework.
 */
void* ooc_interfaceClass();

#endif
