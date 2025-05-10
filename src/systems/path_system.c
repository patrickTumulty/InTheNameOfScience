
#include "path_system.h"
#include "game_math.h"
#include "itnos_components.h"
#include "pathfind_component.h"
#include "pray_entity_registry.h"
#include "pray_system.h"
#include "pray_utils.h"
#include "world_component.h"
#include <raylib.h>

bool isInCircle(Vector2 circleCenter, Vector2 p, float radius)
{
    float deltaX = p.x - circleCenter.x;
    float deltaY = p.y - circleCenter.y;
    return ((deltaX * deltaX) + (deltaY * deltaY)) < (radius * radius);
}

static void moveUnitAlongPath(TransformComponent *transform, PathfindComponent *pathfind)
{
    if (pathfind->active == false)
    {
        return;
    }

    Vector2 *position = &transform->position;

    *position = prayVector2MoveTowards(*position, pathfind->currentPoint, pathfind->speed * GetFrameTime());

    if (isInCircle(pathfind->currentPoint, *position, 1))
    {
        pathfindNextPoint(pathfind);
        if (pathfind->active)
        {
            Vector2 navPoint = pathfind->currentPoint;
            float angle = calculateAngle(navPoint, *position);
            transform->rotationDegrees = angle;
        }
    }
}

static void gameUpdate()
{
    Entity *worldEntity = prayEntityLookup(C(CID_WORLD), 1);
    WorldComponent *worldComponent = prayEntityGetComponent(worldEntity, CID_WORLD);

    LList units;
    Rc rc = prayEntityLookupAll(&units, C(CID_UNIT, CID_TRANSFORM, CID_PATHFINDING, CID_COLLIDER_2D, CID_SPRITE_2D), 3);
    if (rc != RC_OK)
    {
        return;
    }

    LNode *node = nullptr;
    LListForEach(&units, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
        PathfindComponent *pathfind = prayEntityGetComponent(entity, CID_PATHFINDING);
        // UnitComponent *unit = prayEntityGetComponent(entity, CID_UNIT);

        if (pathfind->active)
        {
            moveUnitAlongPath(transform, pathfind);
        }
        else
        {
            // if (unit->roam.t > GetTime())
            // {
            //     float rotation = randomFloat(0, 2 * PI);
            //     float radius = randomFloat(50, 500);
            //     Vector2 newPosition = calculatePointOnCircle(transform->position, rotation, radius);
            // }
        }
    }
}

void registerPathSystem()
{
    System system = {
        .name = "Pathfinding",
        .gameUpdate = gameUpdate,
    };

    praySystemsRegister(system);
}
