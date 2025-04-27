
#include "pointer_list.h"
#include "array_list.h"
#include "common_types.h"


Rc plistNew(PList *plist, u32 length)
{
    return alistNew(plist, length, sizeof(u64));
}

Rc plistAppend(PList *plist, const void *ptr)
{
    u64 temp = (u64) ptr;
    return alistAppend(plist, &temp);
}

Rc plistResize(PList *plist, u32 length)
{
    return alistResize(plist, length);
}

void *plistGet(PList *plist, int index)
{
    u64 *ptr = alistGet(plist, index);
    return (void *) *ptr;
}

Rc plistSet(PList *plist, int index, const void *ptr)
{
    u64 temp = (u64) ptr;
    return alistSet(plist, index, &temp);
}

u32 plistLength(PList *plist)
{
    return alistLength(plist);
}

void plistFree(PList *plist)
{
    return alistFree(plist);
}
