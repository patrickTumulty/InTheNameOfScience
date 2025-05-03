
#include "unit_system.h"
#include "astar.h"
#include "common_types.h"
#include "common_utils.h"
#include "float.h"
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


#define POS2VEC(POS) \
    (Vector2) { .x = (POS).x, .y = (POS).y }
#define VEC2POS(VEC) \
    (Position) { .x = (VEC).x, .y = (VEC).y }

static void clearPath(AStarPath *path)
{
    if (path->path != nullptr)
    {
        tmemfree(path->path);
        path->path = nullptr;
        path->pathLen = 0;
    }
}

#define TEXTURE_ARROW_RED "assets/arrow-red.png"
#define TEXTURE_ARROW_BLUE "assets/arrow-blue.png"

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

static void createEntity(float x, float y, char *texturePath)
{
    Entity *unitEntity = entityNew(C(CID_UNIT, CID_TRANSFORM, CID_PATHFINDING, CID_SPRITE_2D), 4);
    TransformComponent *transform = entityGetComponent(unitEntity, CID_TRANSFORM);

    transform->position.x = x * TILE_SIZE;
    transform->position.y = y * TILE_SIZE;

    Texture2D texture = LoadTexture(texturePath);
    float textureWidth = (float) texture.width;
    float textureHeight = (float) texture.height;

    Sprite2DComponent *sprite2D = entityGetComponent(unitEntity, CID_SPRITE_2D);

    sprite2D->texture = texture;
    sprite2D->source = (Rectangle) {0, 0, textureWidth, textureHeight};
    sprite2D->origin = (Vector2) {textureWidth / 2, textureHeight / 2};
    sprite2D->rotation = 90;

    entityRegistryRegister(unitEntity);
}

static void start()
{
    createEntity(2, 2, TEXTURE_ARROW_BLUE);
    createEntity(10, 10, TEXTURE_ARROW_RED);
}

static void stop()
{
}

Vector2 moveTowards(Vector2 current, Vector2 target, float maxDistanceDelta)
{
    float deltaX = target.x - current.x;
    float deltaY = target.y - current.y;

    float sqDist = (deltaX * deltaX) + (deltaY * deltaY);

    if (feq(sqDist, 0.0f) || (maxDistanceDelta >= 0 && sqDist <= maxDistanceDelta * maxDistanceDelta))
    {
        return target;
    }

    float dist = sqrtf(sqDist);
    return (Vector2) {
        .x = current.x + (deltaX / dist * maxDistanceDelta),
        .y = current.y + (deltaY / dist * maxDistanceDelta),
    };
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

    *position = moveTowards(*position, pathPoint, pathfind->speed * GetFrameTime());

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
    WorldComponent *world = entityGetComponent(worldEntity, CID_WORLD);

    int rows = (int) world->rows;
    int cols = (int) world->cols;

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

        Vector2 position = GetScreenToWorld2D(GetMousePosition(), *getPrayCamera());
        int row = (int) position.y / TILE_SIZE;
        int col = (int) position.x / TILE_SIZE;

        if (inBounds(row, 0, rows) && inBounds(col, 0, cols))
        {
            setPathForPathfindingUnits(world, position, &units);
        }
    }
}

void registerUnitSystem()
{
    System system = {
        .name = "Unit System",
        .start = start,
        .stop = stop,
        .gameUpdate = gameUpdate,
    };

    praySystemsRegister(system);
}
