
#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    int x;
    int y;
} Position;

#define POSEQ(P1, P2) (((P1).x == (P2).x) && ((P1).y == (P2).y))

#define VEC2EQ(V1, V2) (((V1).x == (V2).x) && ((V1).y == (V2).y))

void hexdump(uint8_t *buf, uint32_t bufLen);

typedef enum
{
    OK = 0,
    ERROR = -1,
    MEM_ALLOC_ERROR = -2,
    BAD_PARAM = -3,
} Status;

#endif // UTILS_H
