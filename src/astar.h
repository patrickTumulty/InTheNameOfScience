
#ifndef ASTAR_H
#define ASTAR_H

#include "bool_mat.h"
#include "common_types.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    Position *path;
    uint32_t pathLen;
} AStarPath;

void astar(Position start, Position dest, const BoolMat *navGrid, AStarPath *path);

#endif// ASTAR_H
