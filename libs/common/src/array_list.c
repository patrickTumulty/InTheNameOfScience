
#include "array_list.h"
#include "tmem.h"
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

int arrayListNew(ArrayList *alist, uint32_t length, uint32_t size)
{
    size_t s = (uint64_t) size * length;
    alist->data = tmemcalloc(1, s);
    if (alist->data == nullptr)
    {
        return -1;
    }
    alist->dataSize = s;
    alist->elementSize = (int) size;
    alist->length = (int) length;
    return 0;
}

int arrayListAppend(ArrayList *alist, void *data)
{
    size_t newSize = alist->dataSize + alist->elementSize;
    uint8_t *newBuffer = tmemcalloc(1, newSize);
    if (newBuffer == nullptr)
    {
        return -1;
    }
    memcpy(newBuffer, alist->data, alist->dataSize);
    memcpy(newBuffer + alist->dataSize, data, alist->elementSize);
    tmemfree(alist->data);
    alist->data = newBuffer;
    alist->dataSize += alist->elementSize;
    alist->length++;
    return 0;
}

void *arrayListGet(ArrayList *alist, int index)
{
    if (0 > index || index >= alist->length)
    {
        return nullptr;
    }
    uint32_t offset = alist->elementSize * index;
    return alist->data + offset;
}

int arrayListSet(ArrayList *alist, int index, void *data)
{
    if (0 > index || index >= alist->length)
    {
        return -1;
    }
    uint32_t offset = alist->elementSize * index;
    memcpy(alist->data + offset, data, alist->elementSize);
    return 0;
}

int arrayListLength(ArrayList *alist)
{
    return alist->length;
}

void arrayListFree(ArrayList *alist)
{
    tmemfree(alist->data);
    alist->data = nullptr;
    alist->dataSize = 0;
    alist->elementSize = 0;
    alist->length = 0;
}
