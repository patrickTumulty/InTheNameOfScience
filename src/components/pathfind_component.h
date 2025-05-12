
#ifndef PATHFIND_COMPONENT_H
#define PATHFIND_COMPONENT_H

#include "itnos_components.h"
#include "raylib.h"

void pathfindAddPoint(PathfindComponent *pathfind, Vector2 point);
void pathfindNextPoint(PathfindComponent *pathfind);
void pathfindClearPoints(PathfindComponent *pathfind);

void initPathfindComponent(void *component);
void freePathfindComponent(void *component);

#endif // PATHFIND_COMPONENT_H
