
#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include "common_types.h"
#include <stdint.h>

typedef struct {
    void *data;
    u64 dataSize;
    u32 elementSize;
    u32 length;
} AList;

Rc alistNew(AList *alist, u32 length, u32 size);
Rc alistAppend(AList *alist, const void *data);
Rc alistResize(AList *alist, u32 length);
Rc alistClear(AList *alist);
void *alistGet(AList *alist, int index);
Rc alistSet(AList *alist, int index, const void *data);
u32 alistLength(AList *alist);
void alistFree(AList *alist);

#endif
