
#ifndef UTILS_H
#define UTILS_H

#include "common_types.h"
#include <stddef.h>
#include <stdint.h>

#define MIN(V1, V2) ((V1) < (V2) ? (V1) : (V2))

#define MAX(V1, V2) ((V1) > (V2) ? (V1) : (V2))

#define CLAMP(VAL, MIN_VAL, MAX_VAL) (MIN((MAX_VAL), MAX((MIN_VAL), (VAL))))

#define POSEQ(P1, P2) (((P1).x == (P2).x) && ((P1).y == (P2).y))

#define VEC2EQ(V1, V2) (((V1).x == (V2).x) && ((V1).y == (V2).y))

bool feq(float f1, float f2);

void hexdump(uint8_t *buf, uint32_t bufLen);

bool inBounds(int v, int min, int max);

const char *rc2str(Rc rc);


#endif // UTILS_H
