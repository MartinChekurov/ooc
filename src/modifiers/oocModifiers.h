/**
 * @file oocModifiers.h
 * @brief Class modifier definitions for the OOC (Object-Oriented C) framework
 *
 * This header defines modifier flags that can be applied to classes in the
 * OOC framework, similar to access modifiers and class modifiers in
 * object-oriented languages like Java.
 */

#ifndef OOC_MODIFIERS_H_
#define OOC_MODIFIERS_H_

/**
 * @brief Class modifier enumeration for OOC framework classes
 */
typedef enum OOC_Modifier OOC_Modifier;

/**
 * @enum OOC_Modifier
 * @brief Enumeration of class modifiers that can be applied to OOC classes
 *
 * These modifiers control class behavior and inheritance rules:
 * - NONE: Default modifier, no special restrictions
 * - FINAL: Class cannot be inherited from
 * - ABSTRACT: Class cannot be instantiated directly
 */
enum OOC_Modifier {
  /** @brief No special modifiers applied to the class */
  OOC_MODIFIER_NONE = 0,

  /** @brief Class is final and cannot be inherited from */
  OOC_MODIFIER_FINAL = 1,

  /** @brief Class is abstract and cannot be instantiated directly */
  OOC_MODIFIER_ABSTRACT = 2,
};

#endif
