
#include "projectile_system.h"
#include "common_utils.h"
#include "game_math.h"
#include "itnos_components.h"
#include "linked_list.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_system.h"
#include "pray_utils.h"
#include "projectile_component.h"
#include "raylib.h"


void projectileNew(Vector2 origin, float angle, float damage)
{
    Entity *entity = prayEntityNew(C(CID_TRANSFORM, CID_PROJECTILE), 2);
    TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
    transform->position = origin;
    transform->rotation = angle;
    ProjectileComponent *projectile = prayEntityGetComponent(entity, CID_PROJECTILE);
    projectile->origin = origin;
    projectile->speed = 1000;
    projectile->range = 4000;
    projectile->terminator = calculatePointOnCircle(origin, angle, projectile->range);
    projectile->damage = damage;
    prayEntityRegister(entity);
}

static void gameUpdate()
{
    LList projectiles;
    prayEntityLookupAll(&projectiles, C(CID_TRANSFORM, CID_PROJECTILE), 2);
    if (projectiles.size == 0)
    {
        return;
    }

    LNode *node = nullptr;
    LListForEach(&projectiles, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
        ProjectileComponent *projectile = prayEntityGetComponent(entity, CID_PROJECTILE);
        transform->position = prayVector2MoveTowards(transform->position,
                                                     projectile->terminator,
                                                     projectile->speed * GetFrameTime());
    }

    node = llistPopFront(&projectiles);
    while (node != nullptr)
    {
        Entity *entity = LListGetEntry(node, Entity);
        TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
        ProjectileComponent *projectile = prayEntityGetComponent(entity, CID_PROJECTILE);
        if (prayVector2Equals(transform->position, projectile->terminator))
        {
            prayEntityUnregister(entity);
            prayEntityFree(entity);
        }
        node = llistPopFront(&projectiles);
    }
}

static void renderUpdate()
{
    LList projectiles;
    prayEntityLookupAll(&projectiles, C(CID_TRANSFORM, CID_PROJECTILE), 2);
    LNode *node = nullptr;
    LListForEach(&projectiles, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
        DrawCircle((int) transform->position.x, (int) transform->position.y, 40, ORANGE);
    }
}

void registerProjectileSystem()
{
    System system = {
        .name = "Projectile",
        .gameUpdate = gameUpdate,
        .renderUpdateWorldSpace = renderUpdate,
    };

    praySystemsRegister(system);
}
