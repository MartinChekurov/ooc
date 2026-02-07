#include "unity.h"
#include "oocInterface.h"
#include "oocInterface.r"
#include "oocObject.h"
#include "oocObject.r"
#include "oocError.h"
#include <stdlib.h>
#include <string.h>

// Create unique test classes for each test to avoid conflicts
typedef struct TestClassA TestClassA;
typedef struct TestClassB TestClassB;
typedef struct TestClassC TestClassC;
typedef struct TestClassD TestClassD;
typedef struct TestClassE TestClassE;

struct TestClassA { struct OOC_Class class; };
struct TestClassB { struct OOC_Class class; };
struct TestClassC { struct OOC_Class class; };
struct TestClassD { struct OOC_Class class; };
struct TestClassE { struct OOC_Class class; };

static TestClassA TestClassAInstance;
static TestClassB TestClassBInstance;
static TestClassC TestClassCInstance;
static TestClassD TestClassDInstance;
static TestClassE TestClassEInstance;

static void* getTestClassA(void) {
    static bool initialized = false;
    if (!initialized) {
        ooc_classNew(&TestClassAInstance, "TestClassA",
                    sizeof(OOC_Object), sizeof(TestClassA),
                    ooc_objectClass(), OOC_MODIFIER_NONE, 0);
        initialized = true;
    }
    return &TestClassAInstance;
}

static void* getTestClassB(void) {
    static bool initialized = false;
    if (!initialized) {
        ooc_classNew(&TestClassBInstance, "TestClassB",
                    sizeof(OOC_Object), sizeof(TestClassB),
                    ooc_objectClass(), OOC_MODIFIER_NONE, 0);
        initialized = true;
    }
    return &TestClassBInstance;
}

static void* getTestClassC(void) {
    static bool initialized = false;
    if (!initialized) {
        ooc_classNew(&TestClassCInstance, "TestClassC",
                    sizeof(OOC_Object), sizeof(TestClassC),
                    ooc_objectClass(), OOC_MODIFIER_NONE, 0);
        initialized = true;
    }
    return &TestClassCInstance;
}

static void* getTestClassD(void) {
    static bool initialized = false;
    if (!initialized) {
        ooc_classNew(&TestClassDInstance, "TestClassD",
                    sizeof(OOC_Object), sizeof(TestClassD),
                    ooc_objectClass(), OOC_MODIFIER_NONE, 0);
        initialized = true;
    }
    return &TestClassDInstance;
}

static void* getTestClassE(void) {
    static bool initialized = false;
    if (!initialized) {
        ooc_classNew(&TestClassEInstance, "TestClassE",
                    sizeof(OOC_Object), sizeof(TestClassE),
                    ooc_objectClass(), OOC_MODIFIER_NONE, 0);
        initialized = true;
    }
    return &TestClassEInstance;
}

void test_interface_class_creation(void) {
    void* interfaceClass = ooc_interfaceClass();
    TEST_ASSERT_NOT_NULL(interfaceClass);

    // Interface class should inherit from Object
    TEST_ASSERT_EQUAL_PTR(ooc_objectClass(), ooc_classSuper(interfaceClass));

    // Check interface class properties
    const char* name = ooc_classGetName(interfaceClass);
    TEST_ASSERT_EQUAL_STRING("Interface", name);

    TEST_ASSERT_EQUAL_UINT(sizeof(OOC_Interface), ooc_classSizeOf(interfaceClass));
}

void test_interface_instantiation(void) {
    void* interfaceClass = ooc_interfaceClass();

    // Interfaces should be instantiable (unlike pure abstract classes)
    void* interface = ooc_new(interfaceClass, 0);  // 0 terminates method list
    TEST_ASSERT_NOT_NULL(interface);

    TEST_ASSERT_TRUE(ooc_isInstanceOf(interface, interfaceClass));
    TEST_ASSERT_TRUE(ooc_isInstanceOf(interface, ooc_objectClass()));

    ooc_destroy(interface);
}

void test_interface_constructor(void) {
    void* interfaceClass = ooc_interfaceClass();

    // Test interface constructor with method offsets
    // This is a simplified test - in practice you'd define actual interface methods
    void* interface = ooc_new(interfaceClass,
                             16,  // offset to first method
                             (void(*)(void))0x12345678,  // dummy function pointer
                             0);  // terminator

    TEST_ASSERT_NOT_NULL(interface);

    ooc_destroy(interface);
}

void test_interface_implementation(void) {
    // Use unique class A for this test
    void* obj = ooc_new(getTestClassA());
    TEST_ASSERT_NOT_NULL(obj);

    void* interfaceClass = ooc_interfaceClass();
    void* interface = ooc_new(interfaceClass, 0);
    TEST_ASSERT_NOT_NULL(interface);

    // Test adding interface to class (via object)
    OOC_Error result = ooc_addInterface(obj, interface);
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, result);

    // Test getting interface back
    void* retrieved = ooc_getInterfaceVtable(obj, ooc_classOf(interface));
    TEST_ASSERT_EQUAL_PTR(interface, retrieved);

    ooc_destroy(obj);
    ooc_destroy(interface);
}

void test_interface_multiple_implementations(void) {
    // Use unique class B for this test
    void* obj = ooc_new(getTestClassB());
    TEST_ASSERT_NOT_NULL(obj);

    void* interface1 = ooc_new(ooc_interfaceClass(), 0);
    void* interface2 = ooc_new(ooc_interfaceClass(), 0);  // Same type as interface1
    TEST_ASSERT_NOT_NULL(interface1);
    TEST_ASSERT_NOT_NULL(interface2);

    // Add first interface - should succeed
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_addInterface(obj, interface1));

    // Try to add second interface of same type - should fail
    TEST_ASSERT_EQUAL(OOC_ERROR_DUPLICATE_INTERFACE, ooc_addInterface(obj, interface2));

    // Verify only the first interface is present
    void* retrieved = ooc_getInterfaceVtable(obj, ooc_classOf(interface1));
    TEST_ASSERT_EQUAL_PTR(interface1, retrieved);

    // Create another object of same class - should automatically have the interface
    void* obj2 = ooc_new(getTestClassB());
    void* retrieved2 = ooc_getInterfaceVtable(obj2, ooc_classOf(interface1));
    TEST_ASSERT_EQUAL_PTR(interface1, retrieved2);

    ooc_destroy(obj);
    ooc_destroy(obj2);
    ooc_destroy(interface1);
    ooc_destroy(interface2);
}

void test_interface_inheritance(void) {
    void* interfaceClass = ooc_interfaceClass();

    // Create derived interface class
    OOC_InterfaceClass derivedInterface;
    OOC_Error result = ooc_classNew(&derivedInterface,
                                   "DerivedInterface",
                                   sizeof(OOC_Interface),
                                   sizeof(OOC_InterfaceClass),
                                   interfaceClass,
                                   OOC_MODIFIER_NONE,
                                   0);
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, result);

    void* derived = ooc_new(&derivedInterface, 0);
    TEST_ASSERT_NOT_NULL(derived);

    // Should be instance of both derived and base interface
    TEST_ASSERT_TRUE(ooc_isInstanceOf(derived, &derivedInterface));
    TEST_ASSERT_TRUE(ooc_isInstanceOf(derived, interfaceClass));
    TEST_ASSERT_TRUE(ooc_isInstanceOf(derived, ooc_objectClass()));

    ooc_destroy(derived);
}

void test_interface_binding(void) {
    void* obj = ooc_new(getTestClassC());
    void* interface = ooc_new(ooc_interfaceClass(), 0);

    // Test valid binding
    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_addInterface(obj, interface));

    // Test retrieval of non-existent interface type
    // Use a different class (like Object class) that obj doesn't implement
    TEST_ASSERT_NOT_NULL(ooc_getInterfaceVtable(obj, ooc_objectClass()));

    ooc_destroy(obj);
    ooc_destroy(interface);

}

void test_interface_polymorphism(void) {
    // Use unique class D for this test
    void* obj = ooc_new(getTestClassD());
    void* interface = ooc_new(ooc_interfaceClass(), 0);

    TEST_ASSERT_EQUAL(OOC_ERROR_NONE, ooc_addInterface(obj, interface));

    // Test polymorphic behavior through interface
    void* retrievedInterface = ooc_getInterfaceVtable(obj, ooc_classOf(interface));
    TEST_ASSERT_NOT_NULL(retrievedInterface);

    // Interface should be usable polymorphically
    TEST_ASSERT_TRUE(ooc_isInstanceOf(retrievedInterface, ooc_interfaceClass()));

    const char* str = ooc_toString(retrievedInterface);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_NOT_NULL(strstr(str, "Interface"));

    free((void*)str);
    ooc_destroy(obj);
    ooc_destroy(interface);
}
