
#include "array_list.h"
#include "common_types.h"
#include "common_utils.h"
#include "tmem.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

Rc alistNew(AList *alist, u32 length, u32 size)
{
    size_t s = (u64) size * length;
    if (s == 0)
    {
        alist->data = nullptr;
    }
    else
    {
        alist->data = tmemcalloc(1, s);
        if (alist->data == nullptr)
        {
            return RC_MEM_ALLOC_ERROR;
        }
    }
    alist->dataSize = s;
    alist->elementSize = (int) size;
    alist->length = (int) length;
    return RC_OK;
}

Rc alistResize(AList *alist, u32 length)
{
    if (length == 0)
    {
        return alistClear(alist);
    }
    u64 newSize = (u64) length * alist->elementSize;
    uint8_t *newBuffer = tmemcalloc(1, newSize);
    if (newBuffer == nullptr)
    {
        return RC_MEM_ALLOC_ERROR;
    }
    if (alist->data != nullptr)
    {
        memcpy(newBuffer, alist->data, MIN(alist->dataSize, newSize));
        tmemfree(alist->data);
    }
    alist->data = newBuffer;
    alist->dataSize = newSize;
    alist->length = length;
    return RC_OK;
}

Rc alistAppend(AList *alist, const void *data)
{
    size_t newSize = alist->dataSize + alist->elementSize;
    uint8_t *newBuffer = tmemcalloc(1, newSize);
    if (newBuffer == nullptr)
    {
        return RC_MEM_ALLOC_ERROR;
    }
    if (alist->data != nullptr)
    {
        memcpy(newBuffer, alist->data, alist->dataSize);
        tmemfree(alist->data);
    }
    if (data == nullptr)
    {
        memset(newBuffer + alist->dataSize, 0, alist->elementSize);
    }
    else
    {
        memcpy(newBuffer + alist->dataSize, data, alist->elementSize);
    }
    alist->data = newBuffer;
    alist->dataSize += alist->elementSize;
    alist->length++;
    return RC_OK;
}

void *alistGet(AList *alist, int index)
{
    if (0 > index || index >= alist->length)
    {
        return nullptr;
    }
    uint32_t offset = alist->elementSize * index;
    return alist->data == nullptr ? nullptr : alist->data + offset;
}

Rc alistSet(AList *alist, int index, const void *data)
{
    if (0 > index || index >= alist->length)
    {
        return RC_OUT_OF_BOUNDS;
    }
    u32 offset = alist->elementSize * index;
    if (data == nullptr)
    {
        memset(alist->data + offset, 0, alist->elementSize);
    }
    else
    {
        memcpy(alist->data + offset, data, alist->elementSize);
    }
    return RC_OK;
}

u32 alistLength(AList *alist)
{
    return alist->length;
}

void alistFree(AList *alist)
{
    assert(alist->length > 0 && alist->data != nullptr || alist->length == 0 && alist->data == nullptr);
    if (alist->data != nullptr)
    {
        tmemfree(alist->data);
        alist->data = nullptr;
    }
    alist->dataSize = 0;
    alist->elementSize = 0;
    alist->length = 0;
}

Rc alistClear(AList *alist)
{
    if (alist->data != nullptr)
    {
        tmemfree(alist->data);
        alist->data = nullptr;
    }
    alist->dataSize = 0;
    alist->length = 0;
    return RC_OK;
}
