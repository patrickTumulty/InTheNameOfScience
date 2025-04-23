
#include "array_list.h"
#include "common_tests.h"
#include "tmem.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

typedef struct
{
    int number1;
    int number2;
    char str[10];
} ArrayElement;

int setup(void)
{
    tMemInit();
    return 0;
}

int teardown(void)
{
    tMemDestroy();
    return 0;
}

void alistInitAndFreeTest(void)
{
    ArrayList alist = {};

    int rc = arrayListNew(&alist, 10, sizeof(ArrayElement));
    CU_ASSERT_EQUAL(rc, 0);
    CU_ASSERT_PTR_NOT_NULL(alist.data);
    CU_ASSERT_EQUAL(alist.dataSize, sizeof(ArrayElement) * 10);
    CU_ASSERT_EQUAL(alist.length, 10);
    CU_ASSERT_EQUAL(alist.elementSize, sizeof(ArrayElement));

    arrayListFree(&alist);
    CU_ASSERT_PTR_NULL(alist.data);
    CU_ASSERT_EQUAL(alist.dataSize, 0);
    CU_ASSERT_EQUAL(alist.length, 0);
    CU_ASSERT_EQUAL(alist.elementSize, 0);

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void alistSetAndGetElements(void)
{
    ArrayList alist = {};

    arrayListNew(&alist, 5, sizeof(ArrayElement));

    ArrayElement elements[] = {
        { 1,  5, "Hello"},
        { 5, 10, "World"},
        {10, 20,   "Foo"},
        {15, 30,   "Bar"},
        {20, 60, "!!!!!"},
    };

    for (int i = 0; i < 5; i++)
    {
        arrayListSet(&alist, i, &elements[i]);
    }

    for (int i = 0; i < alist.length; i++)
    {
        ArrayElement *e1 = arrayListGet(&alist, i);
        ArrayElement *e2 = &elements[i];
        CU_ASSERT_PTR_NOT_NULL(e1);
        CU_ASSERT_PTR_NOT_EQUAL(e1, e2);
        CU_ASSERT_EQUAL(memcmp(e1, e2, alist.elementSize), 0);
        e1->number2 = 26;
        CU_ASSERT_NOT_EQUAL(e1->number2, e2->number2);
    }

    ArrayElement *element = arrayListGet(&alist, 10); // out of bounds
    CU_ASSERT_PTR_NULL(element);

    arrayListFree(&alist);
}

void alistAppend(void)
{
    ArrayList alist = {};

    arrayListNew(&alist, 1, sizeof(ArrayElement));

    CU_ASSERT_EQUAL(alist.length, 1);

    ArrayElement elements[] = {
        { 1,  5, "Hello"},
        { 5, 10, "World"},
        {10, 20,   "Foo"},
        {15, 30,   "Bar"},
        {20, 60, "!!!!!"},
    };

    for (int i = 0; i < 5; i++)
    {
        arrayListAppend(&alist, &elements[i]);
        CU_ASSERT_EQUAL(alist.length, i + 2);
        CU_ASSERT_EQUAL(alist.elementSize, sizeof(ArrayElement));
        CU_ASSERT_EQUAL(alist.dataSize, (uint64_t)(alist.elementSize * alist.length));
    }

    ArrayElement zero = {};
    ArrayElement *first = arrayListGet(&alist, 0);
    CU_ASSERT_EQUAL(memcmp(first, &zero, alist.elementSize), 0);

    for (int i = 1; i < alist.length; i++)
    {
        ArrayElement *e1 = arrayListGet(&alist, i);
        ArrayElement *e2 = &elements[i - 1];
        CU_ASSERT_PTR_NOT_NULL(e1);
        CU_ASSERT_PTR_NOT_EQUAL(e1, e2);
        CU_ASSERT_EQUAL(memcmp(e1, e2, alist.elementSize), 0);
        e1->number2 = 26;
        CU_ASSERT_NOT_EQUAL(e1->number2, e2->number2);
    }

    ArrayElement *element = arrayListGet(&alist, 10); // out of bounds
    CU_ASSERT_PTR_NULL(element);

    arrayListFree(&alist);

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void registerArrayListTests(void)
{
    CU_pSuite suite = CU_add_suite("Array List Tests", setup, teardown);
    CU_add_test(suite, "Init array list test", alistInitAndFreeTest);
    CU_add_test(suite, "Setting and Getting Element", alistSetAndGetElements);
    CU_add_test(suite, "Append test", alistAppend);
}
