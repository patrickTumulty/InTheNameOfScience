
#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stdint.h>

typedef struct {
    void *data;
    uint64_t dataSize;
    uint32_t elementSize;
    uint32_t length;
} ArrayList;

int arrayListNew(ArrayList *alist, uint32_t length, uint32_t size);
int arrayListAppend(ArrayList *alist, void *data);
void *arrayListGet(ArrayList *alist, int index);
int arrayListSet(ArrayList *alist, int index, void *data);
int arrayListLength(ArrayList *alist);
void arrayListFree(ArrayList *alist);

#endif
