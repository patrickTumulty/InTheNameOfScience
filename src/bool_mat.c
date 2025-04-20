
#include "bool_mat.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


BoolMat *boolMatNew(int rows, int cols, bool initial)
{
    int size = sizeof(BoolMat) + (rows * sizeof(bool *)) + (rows * cols);
    uint8_t *data = calloc(1, size);
    if (data == NULL)
    {
        return NULL;
    }

    BoolMat *boolMat = (BoolMat *) data;

    boolMat->rows = rows;
    boolMat->cols = cols;

    uint32_t offset = sizeof(BoolMat) + (rows * sizeof(bool *));
    for (int i = 0; i < rows; i++)
    {
        boolMat->mat[i] = (bool *) (data + offset);
        memset(boolMat->mat[i], initial, cols);
        offset += cols;
    }

    return boolMat;
}

BoolMat *boolMatFree(BoolMat *boolMat)
{
    free(boolMat);
    return NULL;
}

BoolMat *boolMatNewCopy(const BoolMat *boolMat)
{
    size_t size = sizeof(BoolMat) + (boolMat->rows * sizeof(bool *)) + (boolMat->rows * boolMat->cols);
    uint8_t *data = calloc(1, size);
    if (data == NULL)
    {
        return NULL;
    }
    memcpy(data, (uint8_t *) boolMat, size);
    return (BoolMat *) data;
}

static bool inBounds(const BoolMat *boolMat, int x, int y)
{
    return (0 <= x && x < boolMat->cols) && (0 <= y && y < boolMat->rows);
}

int boolMatGet(const BoolMat *boolMat, int x, int y)
{
    if (!inBounds(boolMat, x, y))
    {
        return BM_OUT_OF_BOUNDS;
    }
    return boolMat->mat[y][x];
}

int boolMatSet(BoolMat *boolMat, int x, int y, bool b)
{
    if (!inBounds(boolMat, x, y))
    {
        return BM_OUT_OF_BOUNDS;
    }
    boolMat->mat[y][x] = b;
    return b;
}
