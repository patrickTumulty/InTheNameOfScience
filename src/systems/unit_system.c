
#include "unit_system.h"
#include "astar.h"
#include "common_types.h"
#include "common_utils.h"
#include "game_math.h"
#include "itnos_components.h"
#include "linked_list.h"
#include "pray_camera.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_globals.h"
#include "pray_system.h"
#include "raylib.h"
#include "tmem.h"
#include <math.h>
#include <stdio.h>

static void clearPath(AStarPath *path)
{
    if (path->path != nullptr)
    {
        tmemfree(path->path);
        path->path = nullptr;
        path->pathLen = 0;
    }
}

bool isInCircle(Vector2 circleCenter, Vector2 p, float radius)
{
    float deltaX = p.x - circleCenter.x;
    float deltaY = p.y - circleCenter.y;
    return ((deltaX * deltaX) + (deltaY * deltaY)) < (radius * radius);
}

void calculateTrangle(Vector2 center, float roatationDegrees, float radius, Vector2 points[3])
{
    points[0] = calculateRotation(center, DEG2RAD * roatationDegrees, radius);
    points[1] = calculateRotation(center, DEG2RAD * (120 + roatationDegrees), radius);
    points[2] = calculateRotation(center, DEG2RAD * (240 + roatationDegrees), radius);
}

static void start()
{
    Entity *worldEntity = entityRegistryLookupFirst(C(CID_WORLD), 1);
    WorldComponent *worldComponent = entityGetComponent(worldEntity, CID_WORLD);

    Entity *unitEntity = entityNew(C(CID_UNIT, CID_TRANSFORM, CID_PATHFINDING), 3);
    TransformComponent *transform = entityGetComponent(unitEntity, CID_TRANSFORM);
    transform->position.x = (float) (worldComponent->cols / 2) * TILE_SIZE;
    transform->position.y = (float) (worldComponent->rows / 2) * TILE_SIZE;

    entityRegistryRegister(unitEntity);
}

static void stop()
{
}

static void moveUnitAlongPath(TransformComponent *transform, PathfindComponent *pathfind)
{
    if (pathfind->pathSet == false)
    {
        return;
    }

    float half = TILE_SIZE / 2;

    Vector2 *position = &transform->position;
    Position temp = pathfind->path.path[pathfind->index];
    Vector2 pathPoint = {
        .x = (float) (temp.x * TILE_SIZE) + half,
        .y = (float) (temp.y * TILE_SIZE) + half,
    };

    if (pathPoint.x > position->x)
    {
        position->x += 1 * pathfind->speed * GetFrameTime();
    }
    else if (pathPoint.x < position->x)
    {
        position->x -= 1 * pathfind->speed * GetFrameTime();
    }

    if (pathPoint.y > position->y)
    {
        position->y += 1 * pathfind->speed * GetFrameTime();
    }
    else if (pathPoint.y < position->y)
    {
        position->y -= 1 * pathfind->speed * GetFrameTime();
    }

    if (isInCircle(pathPoint, *position, 2))
    {
        pathfind->index++;
        if (pathfind->index == pathfind->path.pathLen)
        {
            clearPath(&pathfind->path);
            pathfind->pathSet = false;
        }
        else
        {
            temp = pathfind->path.path[pathfind->index];
            pathPoint = (Vector2) {
                .x = (float) (temp.x * TILE_SIZE) + half,
                .y = (float) (temp.y * TILE_SIZE) + half,
            };
            float angle = calculateAngle(pathPoint, *position);
            transform->rotation = angle;
        }
    }
}

static void setPathForPathfindingUnits(WorldComponent *world, Vector2 position, LList *units)
{
    LNode *node = nullptr;
    LListForEach(units, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        TransformComponent *transform = entityGetComponent(entity, CID_TRANSFORM);
        PathfindComponent *pathfind = entityGetComponent(entity, CID_PATHFINDING);

        Position start = (Position) {
            .x = (int) transform->position.x / TILE_SIZE,
            .y = (int) transform->position.y / TILE_SIZE,
        };

        Position dest = (Position) {
            .x = (int) position.x / TILE_SIZE,
            .y = (int) position.y / TILE_SIZE,
        };

        clearPath(&pathfind->path);
        pathfind->index = 0;
        pathfind->pathSet = false;

        astar(start, dest, world->navGrid, &pathfind->path);

        if (pathfind->path.path != nullptr && pathfind->path.pathLen > 0)
        {
            for (int i = 0; i < world->rows; i++)
            {
                for (int j = 0; j < world->cols; j++)
                {
                    if (world->world[i][j] == '3' || world->world[i][j] == '2')
                    {
                        world->world[i][j] = 0;
                    }
                }
            }

            pathfind->pathSet = true;

            for (int i = 0; i < pathfind->path.pathLen; i++)
            {
                Position p = pathfind->path.path[i];
                world->world[p.y][p.x] = '3';
            }
        }
    }
}

static void gameUpdate()
{
    Entity *worldEntity = entityRegistryLookupFirst(C(CID_WORLD), 1);
    WorldComponent *worldComponent = entityGetComponent(worldEntity, CID_WORLD);

    int rows = (int) worldComponent->rows;
    int cols = (int) worldComponent->cols;

    LList units;
    Rc rc = entityRegistryLookupAll(&units, C(CID_UNIT, CID_TRANSFORM, CID_PATHFINDING), 3);
    if (rc != RC_OK)
    {
        return;
    }

    LNode *node = nullptr;
    LListForEach(&units, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        TransformComponent *transform = entityGetComponent(entity, CID_TRANSFORM);
        PathfindComponent *pathfind = entityGetComponent(entity, CID_PATHFINDING);

        moveUnitAlongPath(transform, pathfind);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 position = GetScreenToWorld2D(GetMousePosition(), *getPrayCamera());
        int row = (int) position.y / TILE_SIZE;
        int col = (int) position.x / TILE_SIZE;

        if (inBounds(row, 0, rows) && inBounds(col, 0, cols))
        {
            setPathForPathfindingUnits(worldComponent, position, &units);
        }
    }
}

static void renderUpdateWorldSpace()
{
    LList units;
    Rc rc = entityRegistryLookupAll(&units, C(CID_UNIT, CID_TRANSFORM), 2);
    if (rc != RC_OK)
    {
        return;
    }

    LNode *node = nullptr;
    LListForEach(&units, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        TransformComponent *transform = entityGetComponent(entity, CID_TRANSFORM);

        Vector2 points[3];

        calculateTrangle(transform->position, transform->rotation, 30, points);
        DrawCircle((int) points[0].x, (int) points[0].y, 10, BLACK);
        DrawTriangle(points[2], points[1], points[0], RED);
    }
}

void registerUnitSystem()
{
    System system = {
        .name = "Unit System",
        .start = start,
        .stop = stop,
        .gameUpdate = gameUpdate,
        .renderUpdateWorldSpace = renderUpdateWorldSpace,
    };

    praySystemsRegister(system);
}
