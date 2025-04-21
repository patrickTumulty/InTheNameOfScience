
#ifndef TMEM_H
#define TMEM_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint64_t current;
    uint64_t peak;
    uint64_t alltime;
    uint64_t allocations;
    uint64_t frees;
} TMemStats;

void tMemPrintStats();

void tMemInit();
void tMemDestroy();
TMemStats tMemGetStats();
void *tMemCalloc(uint64_t num, uint64_t size);
void *tMemMalloc(uint64_t size);
void tMemFree(void *ptr);

#ifdef TMEM_TRACK
#define tmemcalloc tMemCalloc
#define tmemmalloc tMemMalloc
#define tmemfree tMemFree
#else 
#define tmemcalloc calloc 
#define tmemmalloc malloc 
#define tmemfree free 
#endif

#endif
