
#include "projectile_system.h"
#include "game_math.h"
#include "itnos_components.h"
#include "linked_list.h"
#include "pray_default_components.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_system.h"
#include "pray_utils.h"
#include "raylib.h"

void projectileNew(Vector2 origin, float angle, float damage)
{
    Entity *entity = prayEntityNew(C(CID(Transform2DComponent), CID(ProjectileComponent), CID(Collider2DComponent)), 3);
    Transform2DComponent *transform = getComponent(entity, Transform2DComponent);
    transform->position = origin;
    transform->rotationDegrees = angle;
    ProjectileComponent *projectile = getComponent(entity, ProjectileComponent);
    projectile->origin = origin;
    projectile->speed = 4000;
    projectile->range = 4000;
    projectile->terminator = calculatePointOnCircle(origin, DEG2RAD * angle, projectile->range);
    projectile->damage = damage;
    Collider2DComponent *collider = getComponent(entity, Collider2DComponent);
    collider->type = COLLIDER_2D_CIRCLE;
    collider->radius = 40;
    prayEntityRegister(entity);
}

static void gameUpdate()
{
    LList projectiles;
    prayEntityLookupAll(&projectiles, C(CID(Transform2DComponent), CID(ProjectileComponent), CID(Collider2DComponent)), 3);
    if (projectiles.size == 0)
    {
        return;
    }

    LList targets;
    prayEntityLookupAll(&targets, C(CID(Collider2DComponent), CID(TargetComponent), CID(Transform2DComponent)), 3);

    LNode *projectileNode = nullptr;
    LNode *targetNode = nullptr;
    LListForEach(&projectiles, projectileNode)
    {
        Entity *entity = LListGetEntry(projectileNode, Entity);
        auto transform = getComponent(entity, Transform2DComponent);
        auto projectile = getComponent(entity, ProjectileComponent);
        auto collider = getComponent(entity, Collider2DComponent);
        transform->position = prayVector2MoveTowards(transform->position,
                                                     projectile->terminator,
                                                     projectile->speed * GetFrameTime());

        LListForEach(&targets, targetNode)
        {
            Entity *targetEntity = LListGetEntry(targetNode, Entity);
            auto targetCollider = getComponent(targetEntity, Collider2DComponent);
            auto targetComponent = getComponent(targetEntity, TargetComponent);
            auto targetTransform = getComponent(targetEntity, Transform2DComponent);
            bool collision = CheckCollisionCircles(transform->position, collider->radius, targetTransform->position, targetCollider->radius);
            if (collision && (targetComponent->targetBitmask & projectile->targetBitmask) != 0)
            {
                if (targetComponent->onHitCallback != nullptr)
                {
                    targetComponent->onHitCallback(targetEntity, entity, projectile);
                }
            }
        }
    }

    projectileNode = llistPopFront(&projectiles);
    while (projectileNode != nullptr)
    {
        Entity *entity = LListGetEntry(projectileNode, Entity);
        auto transform = getComponent(entity, Transform2DComponent);
        auto projectile = getComponent(entity, ProjectileComponent);
        if (prayVector2Equals(transform->position, projectile->terminator))
        {
            prayEntityUnregister(entity);
            prayEntityFree(entity);
        }
        projectileNode = llistPopFront(&projectiles);
    }
}

static void renderUpdate()
{
    LList projectiles;
    prayEntityLookupAll(&projectiles, C(CID(Transform2DComponent), CID(ProjectileComponent)), 2);
    LNode *node = nullptr;
    LListForEach(&projectiles, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        auto transform = getComponent(entity, Transform2DComponent);
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
