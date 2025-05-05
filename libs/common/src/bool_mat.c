
#include "bool_mat.h"
#include "common_types.h"
#include "tmem.h"
#include <asm-generic/errno.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


BoolMat *boolMatNew(uint32_t rows, uint32_t cols, bool initial, bool outOfBounds)
{
    u32 bytesPerRow = (u32) ceil((double) cols / 8);
    u8 *data = tmemcalloc(1, sizeof(BoolMat) + (rows * sizeof(u8 *)) + (rows * bytesPerRow));
    if (data == nullptr)
    {
        return nullptr;
    }

    BoolMat *boolMat = (BoolMat *) data;
    boolMat->outOfBounds = outOfBounds;
    boolMat->rows = rows;
    boolMat->cols = cols;

    uint32_t offset = sizeof(BoolMat) + (rows * sizeof(u8 *));
    for (int i = 0; i < rows; i++)
    {
        boolMat->mat[i] = data + offset;
        memset(boolMat->mat[i], initial ? 0xFF : 0x0, bytesPerRow);
        offset += bytesPerRow;
    }

    return boolMat;
}

BoolMat *boolMatFree(BoolMat *boolMat)
{
    tmemfree(boolMat);
    return nullptr;
}

BoolMat *boolMatNewCopy(const BoolMat *boolMat)
{
    u32 bytesPerRow = (u32) ceil((double) boolMat->cols / 8);
    size_t size = sizeof(BoolMat) + (boolMat->rows * sizeof(u8 *)) + (boolMat->rows * bytesPerRow);
    uint8_t *data = tmemcalloc(1, size);
    if (data == nullptr)
    {
        return nullptr;
    }
    memcpy(data, (uint8_t *) boolMat, size);
    return (BoolMat *) data;
}

static bool outOfBounds(const BoolMat *boolMat, int x, int y)
{
    return 0 > x || x >= boolMat->cols || 0 > y || y >= boolMat->rows;
}

bool boolMatGet(const BoolMat *boolMat, int x, int y)
{
    if (outOfBounds(boolMat, x, y))
    {
        return boolMat->outOfBounds;
    }
    int col = (int) floor((float) x / 8);
    u8 targetByte = boolMat->mat[y][col];
    u8 bitmask = 1 << (x % 8);
    return (targetByte & bitmask) != 0 ;
}

void boolMatSet(BoolMat *boolMat, int x, int y, bool b)
{
    if (outOfBounds(boolMat, x, y))
    {
        return;
    }
    int col = (int) floor((float) x / 8);
    u8 targetByte = boolMat->mat[y][col];
    u8 bitmask = 1 << (x % 8);
    if (b)
    {
        u8 new = targetByte | bitmask;
        boolMat->mat[y][col] = new;
    }
    else
    {
        u8 new = targetByte & ~bitmask;
        boolMat->mat[y][col] = new;
    }
}
