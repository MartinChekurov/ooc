/**
* @file oocError.h
 * @brief Error code definitions for the OOC (Object-Oriented C) framework
 *
 * This header defines all error codes used throughout the OOC framework.
 * Error codes are used to indicate various failure conditions and provide
 * standardized error reporting across the framework.
 */

#ifndef OOC_ERROR_H_
#define OOC_ERROR_H_

/**
 * @brief Error code enumeration for OOC framework operations
 */
typedef enum OOC_Error OOC_Error;

/**
 * @enum OOC_Error
 * @brief Enumeration of all possible error codes in the OOC framework
 */
enum OOC_Error {
    /** @brief No error occurred - operation completed successfully */
    OOC_ERROR_NONE = 0,

    /** @brief Memory allocation failed */
    OOC_ERROR_OUT_OF_MEMORY,

    /** @brief Invalid argument passed to function */
    OOC_ERROR_INVALID_ARGUMENT,

    /** @brief Object is not valid or corrupted */
    OOC_ERROR_INVALID_OBJECT,

    /** @brief Requested functionality is not implemented */
    OOC_ERROR_NOT_IMPLEMENTED,

    /** @brief No class descriptor found */
    OOC_ERROR_NO_CLASS,

    /** @brief Attempt to inherit from a final class */
    OOC_ERROR_FINAL_CLASS_INHERITANCE,

    /** @brief Provided buffer is too small for the operation */
    OOC_ERROR_BUFFER_TOO_SMALL,

    /** @brief Standard library error occurred */
    OOC_ERROR_STANDARD,

    /** @brief Interface type already implemented by this object */
    OOC_ERROR_DUPLICATE_INTERFACE,

    /** @brief NULL pointer provided where a valid pointer was expected */
    OOC_ERROR_NULL_POINTER,

    /** @brief Requested interface not implemented by this object */
    OOC_ERROR_INTERFACE_NOT_FOUND,

    /** @brief Operation not supported by this implementation */
    OOC_ERROR_UNSUPPORTED,

    OOC_ERROR_NOT_FOUND,
    OOC_ERROR_DUPLICATE_FOUND,
    OOC_ERROR_INVALID_STATE,
    OOC_ERROR_NOT_SUPPORTED,
    OOC_ERROR_INVALID_MODIFIER,
    OOC_ERROR_NO_PARENT,
    OOC_ERROR_ABSTRACT_METHOD_NOT_IMPLEMENTED,
    /** @brief Total count of error codes (must be last) */
    OOC_ERROR_COUNT
};

#endif
