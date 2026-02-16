#include "unity.h"
#include "oocObject.h"
#include "oocString.h"
#include "oocError.h"
#include "oocHashMapEntry.h"

void test_hash_map_entry_create_get_set_destroy(void) {
    void* key = ooc_new(ooc_stringClass(), "key");
    void* value = ooc_new(ooc_stringClass(), "value");
    void* entry = ooc_new(ooc_hashMapEntryClass(), key, value);

    TEST_ASSERT_NOT_NULL(entry);
    TEST_ASSERT_EQUAL_PTR(key, ooc_hashMapEntryGetKey(entry));
    TEST_ASSERT_EQUAL_PTR(value, ooc_hashMapEntryGetValue(entry));

    void* newKey = ooc_new(ooc_stringClass(), "newKey");
    void* newValue = ooc_new(ooc_stringClass(), "newValue");

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_hashMapEntrySetKey(entry, newKey));
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_hashMapEntrySetValue(entry, newValue));

    TEST_ASSERT_EQUAL_PTR(newKey, ooc_hashMapEntryGetKey(entry));
    TEST_ASSERT_EQUAL_PTR(newValue, ooc_hashMapEntryGetValue(entry));

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_destroy(entry));
}

void test_hash_map_entry_null_and_invalid_argument_handling(void) {
    TEST_ASSERT_NULL(ooc_hashMapEntryGetKey(NULL));
    TEST_ASSERT_NULL(ooc_hashMapEntryGetValue(NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_hashMapEntrySetKey(NULL, NULL));
    TEST_ASSERT_EQUAL(OOC_ERROR_INVALID_ARGUMENT, ooc_hashMapEntrySetValue(NULL, NULL));
}
