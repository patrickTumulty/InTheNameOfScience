
#include "bool_mat.h"
#include "common_tests.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

void initBoolMatFalse(void)
{
    BoolMat *boolMat = boolMatNew(10, 20, false, false);
    CU_ASSERT_PTR_NOT_NULL_FATAL(boolMat);

    CU_ASSERT_EQUAL(boolMat->rows, 10);
    CU_ASSERT_EQUAL(boolMat->cols, 20);
    CU_ASSERT_TRUE(boolMat->outOfBounds == false);

    for (int i = 0; i < boolMat->rows; i++)
    {
        for (int j = 0; j < boolMat->cols; j++)
        {
            CU_ASSERT_EQUAL(boolMatGet(boolMat, j, i), false);
        }
    }

    boolMat = boolMatFree(boolMat);
    CU_ASSERT_PTR_NULL(boolMat);
}

void initBoolMatTrue(void)
{
    BoolMat *boolMat = boolMatNew(10, 20, true, false);
    CU_ASSERT_PTR_NOT_NULL_FATAL(boolMat);

    CU_ASSERT_EQUAL(boolMat->rows, 10);
    CU_ASSERT_EQUAL(boolMat->cols, 20);
    CU_ASSERT_TRUE(boolMat->outOfBounds == false);

    for (int i = 0; i < boolMat->rows; i++)
    {
        for (int j = 0; j < boolMat->cols; j++)
        {
            CU_ASSERT_EQUAL(boolMatGet(boolMat, j, i), true);
        }
    }

    boolMat = boolMatFree(boolMat);
    CU_ASSERT_PTR_NULL(boolMat);
}

void boolMapOutOfBounds(void)
{
    BoolMat *boolMat = boolMatNew(30, 20, true, false);
    CU_ASSERT_PTR_NOT_NULL_FATAL(boolMat);

    CU_ASSERT_EQUAL(boolMat->rows, 30);
    CU_ASSERT_EQUAL(boolMat->cols, 20);
    CU_ASSERT_TRUE(boolMat->outOfBounds == false);

    boolMatSet(boolMat, 29, 19, false);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 18, 28), true);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 19, 29), true);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 20, 30), false);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 1000, 320), false); // Out of bounds
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 10, -1), false);    // Out of bounds
    CU_ASSERT_EQUAL(boolMatGet(boolMat, -32, -20), false);  // Out of bounds
    CU_ASSERT_EQUAL(boolMatGet(boolMat, -32, 1000), false); // Out of bounds
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 0, 0), true);

    boolMat = boolMatFree(boolMat);
    CU_ASSERT_PTR_NULL(boolMat);
}

void setGetTest(void)
{
    BoolMat *boolMat = boolMatNew(10, 10, true, false);
    CU_ASSERT_PTR_NOT_NULL_FATAL(boolMat);

    boolMatSet(boolMat, 5, 5, false);

    CU_ASSERT_EQUAL(boolMatGet(boolMat, 0, 5), true);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 1, 5), true);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 2, 5), true);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 3, 5), true);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 4, 5), true);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 5, 5), false);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 6, 5), true);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 7, 5), true);

    boolMatSet(boolMat, 9, 9, false);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 9, 9), false);

    boolMatSet(boolMat, 9, 9, true);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 9, 9), true);

    boolMatSet(boolMat, 5, 5, true);
    CU_ASSERT_EQUAL(boolMatGet(boolMat, 5, 5), true);


    int counter = 0;
    for (int i = 0; i < boolMat->rows; i++)
    {
        for (int j = 0; j < boolMat->cols; j++)
        {
            if (counter % 2 == 0)
            {
                boolMatSet(boolMat, j, i, false);
            }
            counter++;
        }
    }

    counter = 0;

    for (int i = 0; i < boolMat->rows; i++)
    {
        for (int j = 0; j < boolMat->cols; j++)
        {
            bool expected = true;
            if (counter % 2 == 0)
            {
                expected = false;
            }

            bool value = boolMatGet(boolMat, j, i);
            if (value != expected)
            {
                printf("[i=%2d,j=%2d] value=%5s expected=%5s\n", i, j,
                       value ? "True" : "False",
                       expected ? "True" : "False");
            }
            CU_ASSERT_EQUAL(value, expected);
            counter++;
        }
    }

    boolMat = boolMatFree(boolMat);
    CU_ASSERT_PTR_NULL(boolMat);
}

void registerBoolMatTests(void)
{
    CU_pSuite suite = CU_add_suite("Bool Mat Tests", nullptr, nullptr);
    CU_add_test(suite, "Init Bool Mat (False)", initBoolMatFalse);
    CU_add_test(suite, "Init Bool Mat (True)", initBoolMatTrue);
    CU_add_test(suite, "Out-of-Bounds", boolMapOutOfBounds);
    CU_add_test(suite, "Set/Get Test", setGetTest);
}
