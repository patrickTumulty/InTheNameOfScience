
#include "tmem.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <utils.h>

static TMemStats stats = {0};
static mtx_t lock;

void tMemInit()
{
    memset(&stats, 0, sizeof(TMemStats));
    mtx_init(&lock, mtx_plain);
}

void tMemPrintStats()
{
    TMemStats current = stats;
    printf("tmem stats:\n");
    printf("\tcurrent : %lub\n", current.current);
    printf("\talltime : %lub\n", current.alltime);
    printf("\tpeak    : %lub\n", current.peak);
}

void tMemDestroy()
{
    mtx_destroy(&lock);
}

TMemStats tMemGetStats()
{
    return stats;
}

static void updateGlobalAllocation(uint64_t allocated, uint64_t freed)
{
    mtx_lock(&lock);
    stats.current += allocated;
    stats.alltime += allocated;
    stats.peak = MAX(stats.peak, stats.current);
    stats.current -= freed;
    mtx_unlock(&lock);
}

void *tMemCalloc(uint64_t num, uint64_t size)
{
    uint8_t *mem = calloc(1, (size * num) + sizeof(uint64_t));
    uint64_t *allocSize = (uint64_t *) mem;
    *allocSize = size * num;
    updateGlobalAllocation(size, 0);
    return (mem + sizeof(uint64_t));
}

void *tMemMalloc(uint64_t size)
{
    uint8_t *mem = malloc(size + sizeof(uint64_t));
    uint64_t *allocSize = (uint64_t *) mem;
    *allocSize = size;
    updateGlobalAllocation(size, 0);
    return mem + sizeof(uint64_t);
}

void tMemFree(void *ptr)
{
    uint8_t *mem = (uint8_t *) ptr;
    mem -= sizeof(uint64_t);
    uint64_t *allocSize = (uint64_t *) mem;
    updateGlobalAllocation(0, *allocSize);
    free(mem);
}
