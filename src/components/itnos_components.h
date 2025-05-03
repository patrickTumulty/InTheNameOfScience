
#ifndef ITNOS_COMPONENTS_H
#define ITNOS_COMPONENTS_H

#include "astar.h"
#include "bool_mat.h"
#include "common_types.h"
#include "raylib.h"

typedef struct {
    Texture2D texture; 
    Rectangle source;
    Vector2 origin;
    float rotation; // degrees 
} Sprite2DComponent;

typedef struct
{
    u32 rows;
    u32 cols;
    BoolMat *navGrid;
    char **world;
} WorldComponent;

typedef struct {
    Vector2 position;
    float rotation; // 0.0 to 359.0
} TransformComponent;

typedef enum : u32
{
    CID_RESERVED = 0,
    CID_WORLD,
    CID_TRANSFORM,
    CID_UNIT,
    CID_PATHFINDING,
    CID_SPRITE_2D
} ComponentID;

typedef struct {
    AStarPath path;
    int index;
    float speed;
    bool pathSet;
} PathfindComponent;

#define C(...) (ComponentID[]) { __VA_ARGS__ }

void registerComponents();

#endif // ITNOS_COMPONENTS_H
