
#ifndef WORLD_SYSTEM_H
#define WORLD_SYSTEM_H

#include "bool_mat.h"

typedef struct
{
    int rows;
    int cols;
    BoolMat *navGrid;
    char **world;
} World;

void registerWorldSystem();

#endif // WORLD_SYSTEM_H
