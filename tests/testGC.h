#ifndef TEST_GC_H_
#define TEST_GC_H_

void test_gc_init_shutdown(void);
void test_gc_object_tracking(void);
void test_gc_sweep_unreachable(void);
void test_gc_keep_reachable(void);
void test_gc_multiple_roots(void);
void test_gc_hashmap_with_strings(void);
void test_gc_arraylist_with_strings(void);
void test_gc_nested_collections(void);

#endif
