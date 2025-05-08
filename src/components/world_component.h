
#ifndef WORLD_COMPONENT_H
#define WORLD_COMPONENT_H

#include "bool_mat.h"

typedef struct
{
    u32 rows;
    u32 cols;
    float tileSize;
    BoolMat *navGrid;
    char **world;
} WorldComponent;

void initWorldComponent(void *component);
void deinitWorldComponent(void *component);

#endif // WORLD_COMPONENT_H
