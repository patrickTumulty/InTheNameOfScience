
#ifndef ITNOS_COMPONENTS_H
#define ITNOS_COMPONENTS_H

#include "bool_mat.h"
#include "common_types.h"

typedef struct
{
    u32 rows;
    u32 cols;
    BoolMat *navGrid;
    char **world;
} WorldComponent;

typedef enum : u32
{
    CID_RESERVED = 0,
    CID_WORLD,
    CID_TRANSFORM,
} ComponentID;

#define C(...) (ComponentID[]) { __VA_ARGS__ }

void registerComponents();

#endif // ITNOS_COMPONENTS_H
