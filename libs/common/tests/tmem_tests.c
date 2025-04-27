
#include "common_tests.h"
#include "tmem.h"
#include "common_utils.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdint.h>


typedef struct {
    uint8_t firstByte;
    uint64_t num;
    char str[10];
} TestStruct;

void tMemTest(void)
{
    tMemInit();

    TestStruct *t1 = tMemCalloc(1, sizeof(TestStruct));
    CU_ASSERT_PTR_NOT_NULL_FATAL(t1);

    TMemStats stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, sizeof(TestStruct));
    CU_ASSERT_EQUAL(stats.peak, sizeof(TestStruct));

    TestStruct *t2 = tMemCalloc(1, sizeof(TestStruct));
    CU_ASSERT_PTR_NOT_NULL_FATAL(t1);

    stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, sizeof(TestStruct) * 2);
    CU_ASSERT_EQUAL(stats.peak, sizeof(TestStruct) * 2);

    tMemFree(t2);

    stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, sizeof(TestStruct) * 1);
    CU_ASSERT_EQUAL(stats.peak, sizeof(TestStruct) * 2);

    TestStruct *t3 = tMemCalloc(1, sizeof(TestStruct));
    CU_ASSERT_PTR_NOT_NULL_FATAL(t1);

    stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, sizeof(TestStruct) * 2);
    CU_ASSERT_EQUAL(stats.peak, sizeof(TestStruct) * 2);

    tMemFree(t3);
    tMemFree(t1);

    stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
    CU_ASSERT_EQUAL(stats.peak, sizeof(TestStruct) * 2);

    tMemDestroy();
}

void tMemRedefinition(void)
{
    tMemInit();

    TestStruct *t1 = tmemcalloc(1, sizeof(TestStruct));
    CU_ASSERT_PTR_NOT_NULL_FATAL(t1);

    TMemStats stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, sizeof(TestStruct));
    CU_ASSERT_EQUAL(stats.peak, sizeof(TestStruct));

    TestStruct *t2 = tmemcalloc(1, sizeof(TestStruct));
    CU_ASSERT_PTR_NOT_NULL_FATAL(t1);

    stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, sizeof(TestStruct) * 2);
    CU_ASSERT_EQUAL(stats.peak, sizeof(TestStruct) * 2);

    tmemfree(t2);

    stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, sizeof(TestStruct) * 1);
    CU_ASSERT_EQUAL(stats.peak, sizeof(TestStruct) * 2);

    TestStruct *t3 = tmemcalloc(1, sizeof(TestStruct));
    CU_ASSERT_PTR_NOT_NULL_FATAL(t1);

    stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, sizeof(TestStruct) * 2);
    CU_ASSERT_EQUAL(stats.peak, sizeof(TestStruct) * 2);

    tmemfree(t3);
    tmemfree(t1);

    stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
    CU_ASSERT_EQUAL(stats.peak, sizeof(TestStruct) * 2);

    tMemDestroy();
}

void registerTMemTests(void)
{
    CU_pSuite suite = CU_add_suite("TMem Tests", nullptr, nullptr);
    CU_add_test(suite, "TMem Test", tMemTest);
    CU_add_test(suite, "TMem Redefinition", tMemRedefinition);
}
