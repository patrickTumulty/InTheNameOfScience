
#include "array_list.h"
#include "common_tests.h"
#include "common_types.h"
#include "pointer_list.h"
#include "tmem.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <string.h>
#include <sys/types.h>

typedef struct
{
    int number1;
    int number2;
    char str[10];
} ArrayElement;


static const ArrayElement elements[] = {
    { 1,  5, "Hello"},
    { 5, 10, "World"},
    {10, 20,   "Foo"},
    {15, 30,   "Bar"},
    {20, 60, "!!!!!"},
};

static const int elementsLen = 5;

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
    AList alist = {};

    int rc = alistNew(&alist, 10, sizeof(ArrayElement));
    CU_ASSERT_EQUAL(rc, 0);
    CU_ASSERT_PTR_NOT_NULL(alist.data);
    CU_ASSERT_EQUAL(alist.dataSize, sizeof(ArrayElement) * 10);
    CU_ASSERT_EQUAL(alist.length, 10);
    CU_ASSERT_EQUAL(alist.elementSize, sizeof(ArrayElement));

    alistFree(&alist);
    CU_ASSERT_PTR_NULL(alist.data);
    CU_ASSERT_EQUAL(alist.dataSize, 0);
    CU_ASSERT_EQUAL(alist.length, 0);
    CU_ASSERT_EQUAL(alist.elementSize, 0);

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void alistSetAndGetElements(void)
{
    AList alist = {};

    alistNew(&alist, elementsLen, sizeof(ArrayElement));

    for (int i = 0; i < 5; i++)
    {
        alistSet(&alist, i, &elements[i]);
    }

    for (int i = 0; i < alist.length; i++)
    {
        ArrayElement *e1 = alistGet(&alist, i);
        const ArrayElement *e2 = &elements[i];
        CU_ASSERT_PTR_NOT_NULL(e1);
        CU_ASSERT_PTR_NOT_EQUAL(e1, e2);
        CU_ASSERT_EQUAL(memcmp(e1, e2, alist.elementSize), 0);
        e1->number2 = 26;
        CU_ASSERT_NOT_EQUAL(e1->number2, e2->number2);
    }

    ArrayElement *element = alistGet(&alist, 10); // out of bounds
    CU_ASSERT_PTR_NULL(element);

    alistFree(&alist);

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void alistAppendTest(void)
{
    AList alist = {};

    alistNew(&alist, 1, sizeof(ArrayElement));

    CU_ASSERT_EQUAL(alist.length, 1);

    for (int i = 0; i < elementsLen; i++)
    {
        alistAppend(&alist, &elements[i]);
        CU_ASSERT_EQUAL(alist.length, i + 2);
        CU_ASSERT_EQUAL(alist.elementSize, sizeof(ArrayElement));
        CU_ASSERT_EQUAL(alist.dataSize, (uint64_t) (alist.elementSize * alist.length));
    }

    ArrayElement zero = {};
    ArrayElement *first = alistGet(&alist, 0);
    CU_ASSERT_EQUAL(memcmp(first, &zero, alist.elementSize), 0);

    for (int i = 1; i < alist.length; i++)
    {
        ArrayElement *e1 = alistGet(&alist, i);
        const ArrayElement *e2 = &elements[i - 1];
        CU_ASSERT_PTR_NOT_NULL(e1);
        CU_ASSERT_PTR_NOT_EQUAL(e1, e2);
        CU_ASSERT_EQUAL(memcmp(e1, e2, alist.elementSize), 0);
        e1->number2 = 26;
        CU_ASSERT_NOT_EQUAL(e1->number2, e2->number2);
    }

    ArrayElement *element = alistGet(&alist, 10); // out of bounds
    CU_ASSERT_PTR_NULL(element);

    alistFree(&alist);

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void alistOutOfBoundsTest(void)
{
    AList alist = {};

    alistNew(&alist, elementsLen, sizeof(ArrayElement));

    for (int i = -10; i < 10; i++)
    {
        Rc rc = alistSet(&alist, i, &elements[i]);
        if (i >= 0 && i < elementsLen)
        {
            CU_ASSERT_EQUAL(rc, RC_OK);
        }
        else
        {
            CU_ASSERT_EQUAL(rc, RC_OUT_OF_BOUNDS);
        }
    }

    alistFree(&alist);

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void alistResizeTest(void)
{
    AList alist = {};

    alistNew(&alist, 5, sizeof(ArrayElement));


    for (int i = 0; i < 3; i++)
    {
        Rc rc = alistSet(&alist, i, &elements[i]);
        CU_ASSERT_EQUAL(rc, RC_OK);
    }

    ArrayElement *e0 = alistGet(&alist, 0);
    CU_ASSERT_EQUAL(memcmp(e0, &elements[0], alist.elementSize), 0);
    ArrayElement *e1 = alistGet(&alist, 1);
    CU_ASSERT_EQUAL(memcmp(e1, &elements[1], alist.elementSize), 0);
    ArrayElement *e2 = alistGet(&alist, 2);
    CU_ASSERT_EQUAL(memcmp(e2, &elements[2], alist.elementSize), 0);

    for (int i = 3; i < 5; i++)
    {
        Rc rc = alistSet(&alist, i + 5, &elements[i]);
        CU_ASSERT_EQUAL(rc, RC_OUT_OF_BOUNDS);
    }

    alistResize(&alist, 10);

    e0 = alistGet(&alist, 0);
    CU_ASSERT_EQUAL(memcmp(e0, &elements[0], alist.elementSize), 0);
    e1 = alistGet(&alist, 1);
    CU_ASSERT_EQUAL(memcmp(e1, &elements[1], alist.elementSize), 0);
    e2 = alistGet(&alist, 2);
    CU_ASSERT_EQUAL(memcmp(e2, &elements[2], alist.elementSize), 0);

    for (int i = 3; i < 5; i++)
    {
        Rc rc = alistSet(&alist, i + 5, &elements[i]);
        CU_ASSERT_EQUAL(rc, RC_OK);
    }


    alistResize(&alist, 0);

    for (int i = 0; i < 5; i++)
    {
        ArrayElement *element = alistGet(&alist, i);
        CU_ASSERT_PTR_NULL(element);
    }

    alistFree(&alist);

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

typedef struct
{
    u64 arg1;
    u64 arg2;
    u64 arg3;
    u64 arg4;
    u64 arg5;
} TestStruct;

void plistTest(void)
{
    TestStruct *structs[5];

    for (int i = 0; i < 5; i++)
    {
        TestStruct *ts = tmemcalloc(1, sizeof(TestStruct));
        structs[i] = ts;
        ts->arg1 = 1UL * i;
        ts->arg2 = 2UL * i;
        ts->arg3 = 3UL * i;
        ts->arg4 = 4UL * i;
        ts->arg5 = 5UL * i;
    }

    PList plist = {};
    plistNew(&plist, 5);

    CU_ASSERT_EQUAL(plist.dataSize, sizeof(u64) * 5);
    CU_ASSERT_EQUAL(plist.elementSize, sizeof(u64));
    CU_ASSERT_EQUAL(plist.length, 5);

    for (int i = 0; i < plist.length; i++)
    {
        void *ptr = plistGet(&plist, i);
        CU_ASSERT_PTR_NULL(ptr);
    }

    for (int i = 0; i < plist.length; i++)
    {
        Rc rc = plistSet(&plist, i, structs[i]);
        CU_ASSERT_EQUAL(rc, RC_OK);
    }

    for (int i = 0; i < plist.length; i++)
    {
        TestStruct *ptr = plistGet(&plist, i);
        CU_ASSERT_PTR_NOT_NULL(ptr);
        CU_ASSERT_PTR_EQUAL(ptr, structs[i]);
        CU_ASSERT_EQUAL(ptr->arg1, 1UL * i);
        CU_ASSERT_EQUAL(ptr->arg2, 2UL * i);
        CU_ASSERT_EQUAL(ptr->arg3, 3UL * i);
        CU_ASSERT_EQUAL(ptr->arg4, 4UL * i);
        CU_ASSERT_EQUAL(ptr->arg5, 5UL * i);
    }

    for (int i = 0; i < plist.length; i++)
    {
        TestStruct *ptr = plistGet(&plist, i);
        CU_ASSERT_PTR_NOT_NULL(ptr);
        tmemfree(ptr);
        Rc rc = plistSet(&plist, i, nullptr);
        CU_ASSERT_EQUAL(rc, RC_OK);
    }

    for (int i = 0; i < plist.length; i++)
    {
        void *ptr = plistGet(&plist, i);
        CU_ASSERT_PTR_NULL(ptr);
    }

    plistFree(&plist);

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void emptyThenAdd()
{
    AList alist = {};
    Rc rc = alistNew(&alist, 0, sizeof(u64));
    CU_ASSERT_EQUAL(rc, RC_OK);

    u64 number = 10;
    rc = alistAppend(&alist, &number);
    CU_ASSERT_EQUAL(rc, RC_OK);
}

void registerArrayListTests(void)
{
    CU_pSuite suite = CU_add_suite("Array List Tests", setup, teardown);
    CU_add_test(suite, "Init array list test", alistInitAndFreeTest);
    CU_add_test(suite, "Setting and Getting Element", alistSetAndGetElements);
    CU_add_test(suite, "Append test", alistAppendTest);
    CU_add_test(suite, "Out-of-Bounds test", alistOutOfBoundsTest);
    CU_add_test(suite, "Resize array test", alistResizeTest);
    CU_add_test(suite, "PList test", plistTest);
    CU_add_test(suite, "Empty then append", emptyThenAdd);
}
