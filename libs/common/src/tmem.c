
#include "tmem.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <common_utils.h>

static TMemStats stats = {0};
static mtx_t lock;

typedef struct {
    uint64_t size;
    uint64_t ptr;
} TMemMeta;

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
    printf("\tallocs  : %lu\n", current.allocations);
    printf("\tfrees   : %lu\n", current.frees);
}

void tMemDestroy()
{
    mtx_destroy(&lock);
}

TMemStats tMemGetStats()
{
    return stats;
}

static void updateGlobalAllocationStats(uint64_t allocated)
{
    mtx_lock(&lock);
    stats.current += allocated;
    stats.alltime += allocated;
    stats.peak = MAX(stats.peak, stats.current);
    stats.allocations++;
    mtx_unlock(&lock);
}


static void updateGlobalFreeStats(uint64_t freed)
{
    mtx_lock(&lock);
    stats.current -= freed;
    stats.frees++;
    mtx_unlock(&lock);
}

void *tMemCalloc(uint64_t num, uint64_t size)
{
    uint8_t *mem = calloc(1, (size * num) + sizeof(TMemMeta));
    TMemMeta *meta = (TMemMeta *) mem;
    mem += sizeof(TMemMeta);
    meta->size = size * num;
    meta->ptr = (uint64_t) mem;
    updateGlobalAllocationStats(size);
    return mem;
}

void *tMemMalloc(uint64_t size)
{
    uint8_t *mem = malloc(size + sizeof(TMemMeta));
    TMemMeta *meta = (TMemMeta *) mem;
    mem += sizeof(TMemMeta);
    meta->size = size;
    meta->ptr = (uint64_t) mem;
    updateGlobalAllocationStats(size);
    return mem;
}

void tMemFree(void *ptr)
{
    uint8_t *mem = (uint8_t *) ptr;
    mem -= sizeof(TMemMeta);
    TMemMeta *meta = (TMemMeta *) mem;
    assert(meta->ptr == (uint64_t) ptr);
    updateGlobalFreeStats(meta->size);
    free(mem);
}
