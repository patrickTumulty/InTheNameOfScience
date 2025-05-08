
#include "pathfind_component.h"
#include "linked_list.h"
#include "raylib.h"
#include "tmem.h"

typedef struct
{
    Vector2 point;
    LNode node;
} PathElement;


void initPathfindComponent(void *component)
{
    PathfindComponent *pathfind = (PathfindComponent *) component;
    llistInit(&pathfind->path);
    pathfind->active = false;
    pathfind->index = 0;
    pathfind->speed = 1200;
}

void freePathfindComponent(void *component)
{
    PathfindComponent *pathfind = (PathfindComponent *) component;
    pathfindClearPoints(pathfind);
}

void pathfindNextPoint(PathfindComponent *pathfind)
{
    LNode *node = llistPopFront(&pathfind->path);
    if (node == nullptr)
    {
        pathfind->active = false;
        return;
    }
    PathElement *element = LListGetEntry(node, PathElement);
    pathfind->currentPoint = element->point;
    pathfind->active = true;
    tmemfree(element);
}

void pathfindAddPoint(PathfindComponent *pathfind, const Vector2 point)
{
    PathElement *newPathElement = tmemcalloc(1, sizeof(PathElement));
    newPathElement->point = point;
    llistInitNode(&newPathElement->node, newPathElement);
    llistAppend(&pathfind->path, &newPathElement->node);
    pathfind->currentPoint = LListGetEntry(pathfind->path.head, PathElement)->point;
    pathfind->active = true;
}

void pathfindClearPoints(PathfindComponent *pathfind)
{
    LNode *node = llistPopFront(&pathfind->path);
    while (node != nullptr)
    {
        tmemfree(node->data);
        node = llistPopFront(&pathfind->path);
    }
    pathfind->active = false;
}
