#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestRun.h>
#include "pray_engine_tests.h"
#include "tmem.h"
#include <stdint.h>


int main()
{
    tMemInit();

    CU_initialize_registry();

    registerComponentsTests();
    registerEntityTests();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    uint32_t failures = CU_get_number_of_failures();

    CU_cleanup_registry();

    tMemDestroy();

    return failures != 0 ? -1 : 0;
}
