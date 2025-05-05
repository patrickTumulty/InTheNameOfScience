
#ifndef BOOL_MAT_H
#define BOOL_MAT_H

#include "common_types.h"
#include <stdint.h>

typedef struct {
    uint32_t rows;
    uint32_t cols;
    bool outOfBounds; // Return this when out-of-bounds on get
    u8 *mat[];
} BoolMat;

BoolMat *boolMatNew(uint32_t rows, uint32_t cols, bool initial, bool outOfBounds);
BoolMat *boolMatFree(BoolMat *boolMat);
BoolMat *boolMatNewCopy(const BoolMat *boolMat);
bool boolMatGet(const BoolMat *boolMat, int x, int y); 
void boolMatSet(BoolMat *boolMat, int x, int y, bool b); 

#endif // BOOL_MAT_H
