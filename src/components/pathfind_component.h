
#ifndef PATHFIND_COMPONENT_H
#define PATHFIND_COMPONENT_H

#include "linked_list.h"
#include "raylib.h"

typedef struct
{
    LList path;
    Vector2 currentPoint;
    bool active;
    int index;
    float speed;
} PathfindComponent;

void pathfindAddPoint(PathfindComponent *pathfind, Vector2 point);
void pathfindNextPoint(PathfindComponent *pathfind);
void pathfindClearPoints(PathfindComponent *pathfind);

void initPathfindComponent(void *component);
void freePathfindComponent(void *component);

#endif // PATHFIND_COMPONENT_H
