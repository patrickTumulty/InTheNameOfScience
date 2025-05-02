
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestRun.h>
#include <stdint.h>
#include "common_tests.h"

int main()
{
    CU_initialize_registry();

    registerLinkedListTests();
    registerArrayListTests();
    registerBoolMatTests();
    registerTMemTests();
    registerUtilsTests();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    uint32_t failures = CU_get_number_of_failures();

    CU_cleanup_registry();

    return failures != 0 ? -1 : 0;
}
