#include "unity.h"
#include "testObject.h"
#include "testString.h"
#include "testStringBuffer.h"
#include "testArrayList.h"
#include "testLinkedList.h"
#include "testHashSet.h"
#include "testLinkedHashSet.h"
#include "testHashMap.h"
#include "testLinkedHashMap.h"
#include "testArrayDeque.h"
#include "testCoreInterfaces.h"
#include "testHashMapEntry.h"
#include "testGC.h"
#include "testNumber.h"

void setUp(void) {
}

void tearDown(void) {
}

int main(void) {
    UNITY_BEGIN();

    /* Core interface and abstract class tests */
    RUN_TEST(test_interface_and_abstract_class_singletons);
    RUN_TEST(test_collection_interface_null_safety);
    RUN_TEST(test_list_interface_null_safety);
    RUN_TEST(test_set_interface_null_safety);
    RUN_TEST(test_queue_and_deque_interface_null_safety);
    RUN_TEST(test_iterator_interface_null_safety);
    RUN_TEST(test_map_interface_null_safety);
    RUN_TEST(test_interface_non_implementer_behavior);

    /* HashMapEntry tests */
    RUN_TEST(test_hash_map_entry_create_get_set_destroy);
    RUN_TEST(test_hash_map_entry_null_and_invalid_argument_handling);

    /* Object tests */
    RUN_TEST(test_object_class_creation);
    RUN_TEST(test_object_creation_and_destruction);
    RUN_TEST(test_object_class_info);
    RUN_TEST(test_object_instanceof);
    RUN_TEST(test_object_equals);
    RUN_TEST(test_object_hash_code);
    RUN_TEST(test_object_to_string);
    RUN_TEST(test_object_clone);
    RUN_TEST(test_object_destroy_null);

    /* String tests */
    RUN_TEST(test_string_create_destroy);
    RUN_TEST(test_string_length);
    RUN_TEST(test_string_char_at);
    RUN_TEST(test_string_is_empty);
    RUN_TEST(test_string_substring);
    RUN_TEST(test_string_concat);
    RUN_TEST(test_string_starts_with);
    RUN_TEST(test_string_ends_with);
    RUN_TEST(test_string_index_of_char);
    RUN_TEST(test_string_last_index_of_char);
    RUN_TEST(test_string_index_of_string);
    RUN_TEST(test_string_replace_char);
    RUN_TEST(test_string_to_lower_upper);
    RUN_TEST(test_string_trim);
    RUN_TEST(test_string_contains);
    RUN_TEST(test_string_to_cstring);
    RUN_TEST(test_string_equals_and_hash);
    RUN_TEST(test_string_clone);
    RUN_TEST(test_string_static_from_int);
    RUN_TEST(test_string_static_from_bool);
    RUN_TEST(test_string_static_length);
    RUN_TEST(test_string_static_char_at);
    RUN_TEST(test_string_static_substring);
    RUN_TEST(test_string_static_concat);
    RUN_TEST(test_string_static_index_of);
    RUN_TEST(test_string_static_format_and_convert_helpers);
    RUN_TEST(test_string_static_buffer_operations);
    RUN_TEST(test_string_static_mutating_helpers);

    /* StringBuffer tests */
    RUN_TEST(test_string_buffer_create_destroy);
    RUN_TEST(test_string_buffer_length_and_empty);
    RUN_TEST(test_string_buffer_append_cstring);
    RUN_TEST(test_string_buffer_append_char);
    RUN_TEST(test_string_buffer_append_int);
    RUN_TEST(test_string_buffer_append_bool);
    RUN_TEST(test_string_buffer_char_at);
    RUN_TEST(test_string_buffer_set_char_at);
    RUN_TEST(test_string_buffer_substring);
    RUN_TEST(test_string_buffer_insert_cstring);
    RUN_TEST(test_string_buffer_delete);
    RUN_TEST(test_string_buffer_delete_char_at);
    RUN_TEST(test_string_buffer_replace_cstring);
    RUN_TEST(test_string_buffer_index_of);
    RUN_TEST(test_string_buffer_starts_ends_contains);
    RUN_TEST(test_string_buffer_to_cstring);
    RUN_TEST(test_string_buffer_clear);
    RUN_TEST(test_string_buffer_reverse);
    RUN_TEST(test_string_buffer_trim);
    RUN_TEST(test_string_buffer_case);
    RUN_TEST(test_string_buffer_capacity);

    /* ArrayList tests */
    RUN_TEST(test_array_list_create_destroy);
    RUN_TEST(test_array_list_add_and_size);
    RUN_TEST(test_array_list_get_at);
    RUN_TEST(test_array_list_set_at);
    RUN_TEST(test_array_list_insert_at);
    RUN_TEST(test_array_list_remove_at);
    RUN_TEST(test_array_list_contains);
    RUN_TEST(test_array_list_contains_all);
    RUN_TEST(test_array_list_index_of);
    RUN_TEST(test_array_list_last_index_of);
    RUN_TEST(test_array_list_clear);
    RUN_TEST(test_array_list_iterator);
    RUN_TEST(test_array_list_iterator_remove);
    RUN_TEST(test_array_list_list_iterator);
    RUN_TEST(test_array_list_list_iterator_at);
    RUN_TEST(test_array_list_equals);
    RUN_TEST(test_array_list_hash_code);
    RUN_TEST(test_array_list_to_string);
    RUN_TEST(test_array_list_clone);
    RUN_TEST(test_array_list_capacity);
    RUN_TEST(test_array_list_empty_list);
    RUN_TEST(test_array_list_interface_dispatch);
    RUN_TEST(test_array_list_remove_by_value);
    RUN_TEST(test_array_list_allows_null_elements);

    /* LinkedList tests */
    RUN_TEST(test_linked_list_create_destroy);
    RUN_TEST(test_linked_list_add_and_size);
    RUN_TEST(test_linked_list_get_at);
    RUN_TEST(test_linked_list_set_at);
    RUN_TEST(test_linked_list_insert_at);
    RUN_TEST(test_linked_list_remove_at);
    RUN_TEST(test_linked_list_contains);
    RUN_TEST(test_linked_list_index_of);
    RUN_TEST(test_linked_list_last_index_of);
    RUN_TEST(test_linked_list_clear);
    RUN_TEST(test_linked_list_iterator);
    RUN_TEST(test_linked_list_iterator_remove);
    RUN_TEST(test_linked_list_list_iterator);
    RUN_TEST(test_linked_list_equals_with_arraylist);
    RUN_TEST(test_linked_list_queue_operations);
    RUN_TEST(test_linked_list_deque_operations);
    RUN_TEST(test_linked_list_stack_operations);
    RUN_TEST(test_linked_list_empty_deque_edge_cases);
    RUN_TEST(test_linked_list_clone);
    RUN_TEST(test_linked_list_to_string);
    RUN_TEST(test_linked_list_hash_code);
    RUN_TEST(test_linked_list_remove_by_value);
    RUN_TEST(test_linked_list_allows_null_elements);

    /* HashSet tests */
    RUN_TEST(test_hash_set_create_destroy);
    RUN_TEST(test_hash_set_add_and_size);
    RUN_TEST(test_hash_set_no_duplicates);
    RUN_TEST(test_hash_set_contains);
    RUN_TEST(test_hash_set_remove);
    RUN_TEST(test_hash_set_clear);
    RUN_TEST(test_hash_set_iterator);
    RUN_TEST(test_hash_set_iterator_remove);
    RUN_TEST(test_hash_set_contains_all);
    RUN_TEST(test_hash_set_equals);
    RUN_TEST(test_hash_set_hash_code);
    RUN_TEST(test_hash_set_to_string);
    RUN_TEST(test_hash_set_clone);
    RUN_TEST(test_hash_set_empty);
    RUN_TEST(test_hash_set_allows_single_null_element);

    /* LinkedHashSet tests */
    RUN_TEST(test_linked_hash_set_create_destroy);
    RUN_TEST(test_linked_hash_set_insertion_order);
    RUN_TEST(test_linked_hash_set_no_duplicates);
    RUN_TEST(test_linked_hash_set_remove_and_readd);
    RUN_TEST(test_linked_hash_set_equals_with_hash_set);
    RUN_TEST(test_linked_hash_set_clone);
    RUN_TEST(test_linked_hash_set_to_string);
    RUN_TEST(test_linked_hash_set_clear);
    RUN_TEST(test_linked_hash_set_contains);
    RUN_TEST(test_linked_hash_set_iterator_remove);
    RUN_TEST(test_linked_hash_set_allows_single_null_element);

    /* HashMap tests */
    RUN_TEST(test_hash_map_create_destroy);
    RUN_TEST(test_hash_map_put_get_single);
    RUN_TEST(test_hash_map_put_get_multiple);
    RUN_TEST(test_hash_map_put_overwrite);
    RUN_TEST(test_hash_map_contains_key);
    RUN_TEST(test_hash_map_contains_value);
    RUN_TEST(test_hash_map_remove);
    RUN_TEST(test_hash_map_size_and_empty);
    RUN_TEST(test_hash_map_clear);
    RUN_TEST(test_hash_map_iterator);
    RUN_TEST(test_hash_map_iterator_remove);
    RUN_TEST(test_hash_map_key_set);
    RUN_TEST(test_hash_map_values);
    RUN_TEST(test_hash_map_equals);
    RUN_TEST(test_hash_map_hash_code);
    RUN_TEST(test_hash_map_to_string);
    RUN_TEST(test_hash_map_clone);
    RUN_TEST(test_hash_map_supports_null_key_and_value);

    /* LinkedHashMap tests */
    RUN_TEST(test_linked_hash_map_create_destroy);
    RUN_TEST(test_linked_hash_map_insertion_order);
    RUN_TEST(test_linked_hash_map_update_preserves_order);
    RUN_TEST(test_linked_hash_map_remove_and_readd);
    RUN_TEST(test_linked_hash_map_equals_with_hash_map);
    RUN_TEST(test_linked_hash_map_clone);
    RUN_TEST(test_linked_hash_map_clear);
    RUN_TEST(test_linked_hash_map_key_set_order);
    RUN_TEST(test_linked_hash_map_values_order);
    RUN_TEST(test_linked_hash_map_iterator_remove);
    RUN_TEST(test_linked_hash_map_supports_null_key_and_value);

    /* ArrayDeque tests */
    RUN_TEST(test_array_deque_create_destroy);
    RUN_TEST(test_array_deque_queue_operations);
    RUN_TEST(test_array_deque_deque_operations);
    RUN_TEST(test_array_deque_stack_operations);
    RUN_TEST(test_array_deque_contains);
    RUN_TEST(test_array_deque_remove_by_value);
    RUN_TEST(test_array_deque_size_and_clear);
    RUN_TEST(test_array_deque_iterator);
    RUN_TEST(test_array_deque_empty_edge_cases);
    RUN_TEST(test_array_deque_to_string);
    RUN_TEST(test_array_deque_clone);
    RUN_TEST(test_array_deque_add_many);
    RUN_TEST(test_array_deque_allows_null_elements);

    /* GC tests */
    RUN_TEST(test_gc_init_shutdown);
    RUN_TEST(test_gc_object_tracking);
    RUN_TEST(test_gc_sweep_unreachable);
    RUN_TEST(test_gc_keep_reachable);
    RUN_TEST(test_gc_multiple_roots);
    RUN_TEST(test_gc_hashmap_with_strings);
    RUN_TEST(test_gc_arraylist_with_strings);
    RUN_TEST(test_gc_nested_collections);
    RUN_TEST(test_gc_scoped_root_macro);
    RUN_TEST(test_gc_new_rooted_macro);
    RUN_TEST(test_gc_new_scoped_macro);
    RUN_TEST(test_gc_new_in_across_functions);
    RUN_TEST(test_gc_move_root_to_caller_slot);
    RUN_TEST(test_gc_duplicate_root_registration_is_ignored);
    RUN_TEST(test_gc_root_introspection_helpers);

    /* Number tests */
    RUN_TEST(test_number_char);
    RUN_TEST(test_number_uchar);
    RUN_TEST(test_number_short);
    RUN_TEST(test_number_ushort);
    RUN_TEST(test_number_integer);
    RUN_TEST(test_number_uinteger);
    RUN_TEST(test_number_long);
    RUN_TEST(test_number_ulong);
    RUN_TEST(test_number_float);
    RUN_TEST(test_number_double);
    RUN_TEST(test_number_instanceof);

    return UNITY_END();
}
