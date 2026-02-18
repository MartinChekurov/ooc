/**
 * @file oocNumber.h
 * @brief Abstract base class for numeric types in the OOC framework
 */

#ifndef OOC_NUMBER_H_
#define OOC_NUMBER_H_

/**
 * @brief Returns the Number class descriptor
 * @return Pointer to the Number class object
 */
void* ooc_numberClass(void);

int            ooc_numberIntValue(const void* self);
unsigned int   ooc_numberUIntValue(const void* self);
long           ooc_numberLongValue(const void* self);
unsigned long  ooc_numberULongValue(const void* self);
float          ooc_numberFloatValue(const void* self);
double         ooc_numberDoubleValue(const void* self);

#endif
