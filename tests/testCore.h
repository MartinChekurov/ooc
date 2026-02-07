
#ifndef TEST_CORE_H_
#define TEST_CORE_H_

// Core framework tests
void test_core_null_handling(void);
void test_core_error_codes(void);
void test_core_memory_allocation_failure(void);

// Class system tests  
void test_class_creation_invalid_args(void);
void test_class_inheritance_chain(void);
void test_class_method_inheritance(void);
void test_class_final_inheritance_prevention(void);
void test_class_abstract_instantiation_prevention(void);
void test_class_introspection(void);
void test_class_modifier_combinations(void);

// Object lifecycle tests
void test_object_constructor_failure(void);
void test_object_destructor_failure(void);
void test_object_double_destruction(void);
void test_object_null_destruction(void);

// Polymorphism tests
void test_method_dispatch(void);
void test_virtual_method_calls(void);
void test_super_method_calls(void);

#endif
