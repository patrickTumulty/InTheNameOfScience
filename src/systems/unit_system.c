
#include "unit_system.h"
#include "astar.h"
#include "common_types.h"
#include "common_utils.h"
#include "float.h"
#include "game_math.h"
#include "itnos_components.h"
#include "linked_list.h"
#include "pathfind_component.h"
#include "pointer_list.h"
#include "pray_camera.h"
#include "pray_default_components.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_system.h"
#include "projectile_system.h"
#include "raylib.h"
#include "selection_system.h"
#include "tmem.h"
#include "world_component.h"
#include <stdatomic.h>
#include <threads.h>

#define VISION_CIRCLE 1000

static WorldComponent *world;

static void start()
{
    Entity *worldEntity = prayEntityLookup(C(CID(WorldComponent)), 1);
    world = getComponent(worldEntity, WorldComponent);
}

static void setPathForSelectedUnits(WorldComponent *world, Vector2 position)
{
    if (!selectionEntitiesSelected())
    {
        return;
    }

    PList selectedEntities;

    selectionGetSelectedEntities(&selectedEntities);
    float count = 0;
    Vector2 point;

    for (int i = 0; i < selectedEntities.length; i++)
    {
        Entity *entity = plistGet(&selectedEntities, i);
        auto transform = getComponent(entity, Transform2DComponent);
        auto pathfind = getComponent(entity, PathfindComponent);

        point = position;

        Position start = (Position) {
            .x = (int) (transform->position.x / world->tileSize),
            .y = (int) (transform->position.y / world->tileSize),
        };

        Position dest = (Position) {
            .x = (int) (point.x / world->tileSize),
            .y = (int) (point.y / world->tileSize),
        };

        if (!feq(count, 0.0f))
        {
            point = calculatePointOnCircle(position, count * (PI / 4), 250);
            dest = (Position) {
                .x = (int) (point.x / world->tileSize),
                .y = (int) (point.y / world->tileSize),
            };
        }
        count += 1;

        pathfindClearPoints(pathfind);

        AStarPath path;

        astar(start, dest, world->navGrid, &path);

        if (path.pathLen <= 0)
        {
            tmemfree(path.path);
            continue;
        }

        float half = world->tileSize / 2;
        for (int i = 0; i < path.pathLen; i++)
        {
            Position p = path.path[i];
            Vector2 navPoint = {
                .x = ((float) p.x * world->tileSize) + half,
                .y = ((float) p.y * world->tileSize) + half,
            };
            pathfindAddPoint(pathfind, navPoint);
            world->world[p.y][p.x] = '3';
        }

        pathfindAddPoint(pathfind, point);

        tmemfree(path.path);
    }
}

static void renderUpdate()
{
    LList units;
    Rc rc = prayEntityLookupAll(&units, C(CID(Transform2DComponent), CID(Collider2DComponent)), 2);

    LNode *node = nullptr;
    LListForEach(&units, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        Transform2DComponent *transform = getComponent(entity, Transform2DComponent);
        Collider2DComponent *collider2D = getComponent(entity, Collider2DComponent);

        Color green = GREEN;
        green.a = 120;

        DrawCircle((int) (transform->position.x + collider2D->offset.x),
                   (int) (transform->position.y + collider2D->offset.y),
                   collider2D->radius,
                   green);
    }

    rc = prayEntityLookupAll(&units, C(CID(Transform2DComponent), CID(UnitComponent)), 2);
    node = nullptr;
    LListForEach(&units, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        Transform2DComponent *transform = getComponent(entity, Transform2DComponent);

        DrawCircleLines((int) transform->position.x, (int) transform->position.y, VISION_CIRCLE, YELLOW);
    }
}

static void clearAStarPath(WorldComponent *world)
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
}

static void gameUpdate()
{
    int rows = (int) world->rows;
    int cols = (int) world->cols;

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        clearAStarPath(world);

        Vector2 position = GetScreenToWorld2D(GetMousePosition(), *prayGetCamera());
        int row = (int) (position.y / world->tileSize);
        int col = (int) (position.x / world->tileSize);

        if (inBounds(row, 0, rows) && inBounds(col, 0, cols))
        {
            setPathForSelectedUnits(world, position);
        }
    }

    if (IsKeyPressed(KEY_P))
    {
        Entity *entity = prayEntityLookup(C(CID(UnitComponent)), 1);
        auto transform = getComponent(entity, Transform2DComponent);

        projectileNew(transform->position, transform->rotationDegrees, 10);
    }

    // LList enemies;
    // prayEntityLookupAll(&enemies, C(CID_ENEMY), 1);
    // LNode *enemyNode = nullptr;
    //
    // LList units;
    // prayEntityLookupAll(&units, C(CID_UNIT), 1);
    // LNode *unitNode = nullptr;
    //
    // LListForEach(&units, unitNode)
    // {
    //     Entity *unitEntity = LListGetEntry(unitNode, Entity);
    //     TransformComponent *unitTransform = prayEntityGetComponent(unitEntity, CID_TRANSFORM);
    //     TargetingComponent *unitTargeting = prayEntityGetComponent(unitEntity, CID_TARGETING);
    //     PathfindComponent *unitPathfind = prayEntityGetComponent(unitEntity, CID_PATHFINDING);
    //
    //     if (unitTargeting->target != nullptr)
    //     {
    //         continue; // We already have a target
    //     }
    //
    //     LListForEach(&enemies, enemyNode)
    //     {
    //         Entity *enemyEntity = LListGetEntry(enemyNode, Entity);
    //         TransformComponent *enemyTransform = prayEntityGetComponent(enemyEntity, CID_TRANSFORM);
    //         Collider2DComponent *enemyCollider = prayEntityGetComponent(enemyEntity, CID_COLLIDER_2D);
    //
    //         if (CheckCollisionCircles(unitTransform->position, VISION_CIRCLE, enemyTransform->position, enemyCollider->radius))
    //         {
    //             pathfindClearPoints(unitPathfind);
    //             unitTransform->rotation = calculateAngle(enemyTransform->position, unitTransform->position);
    //             break;
    //         }
    //     }
    // }
}

void registerUnitSystem()
{
    System system = {
        .name = "Unit System",
        .start = start,
        .gameUpdate = gameUpdate,
        .renderUpdateWorldSpace = renderUpdate,
    };

    praySystemsRegister(system);
}
