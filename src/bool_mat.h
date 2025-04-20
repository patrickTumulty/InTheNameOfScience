
#ifndef BOOL_MAT_H
#define BOOL_MAT_H

#include <stdint.h>

typedef struct {
    uint32_t rows;
    uint32_t cols;
    bool *mat[];
} BoolMat;

#define BM_OUT_OF_BOUNDS (-1)

BoolMat *boolMatNew(int rows, int cols, bool initial);
BoolMat *boolMatFree(BoolMat *boolMat);
BoolMat *boolMatNewCopy(const BoolMat *boolMat);
int boolMatGet(const BoolMat *boolMat, int x, int y); 
int boolMatSet(BoolMat *boolMat, int x, int y, bool b); 

#endif // BOOL_MAT_H
