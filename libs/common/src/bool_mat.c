
#include "bool_mat.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


BoolMat *boolMatNew(int rows, int cols, bool initial, bool outOfBounds)
{
    uint8_t *data = calloc(1, sizeof(BoolMat) + (rows * sizeof(bool *)) + (rows * cols));
    if (data == nullptr)
    {
        return nullptr;
    }

    BoolMat *boolMat = (BoolMat *) data;
    boolMat->outOfBounds = outOfBounds;

    boolMat->rows = rows;
    boolMat->cols = cols;

    uint32_t offset = sizeof(BoolMat) + (rows * sizeof(bool *));
    for (int i = 0; i < rows; i++)
    {
        boolMat->mat[i] = (bool *) (data + offset);
        memset(boolMat->mat[i], (int) initial, cols);
        offset += cols;
    }

    return boolMat;
}

BoolMat *boolMatFree(BoolMat *boolMat)
{
    free(boolMat);
    return nullptr;
}

BoolMat *boolMatNewCopy(const BoolMat *boolMat)
{
    size_t size = sizeof(BoolMat) + (boolMat->rows * sizeof(bool *)) + (boolMat->rows * boolMat->cols);
    uint8_t *data = calloc(1, size);
    if (data == nullptr)
    {
        return nullptr;
    }
    memcpy(data, (uint8_t *) boolMat, size);
    return (BoolMat *) data;
}

static bool inBounds(const BoolMat *boolMat, int x, int y)
{
    return (0 <= x && x < boolMat->cols) && (0 <= y && y < boolMat->rows);
}

bool boolMatGet(const BoolMat *boolMat, int x, int y)
{
    if (!inBounds(boolMat, x, y))
    {
        return boolMat->outOfBounds;
    }
    return boolMat->mat[y][x];
}

void boolMatSet(BoolMat *boolMat, int x, int y, bool b)
{
    if (!inBounds(boolMat, x, y))
    {
        return;
    }
    boolMat->mat[y][x] = b;
}
