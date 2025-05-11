
#include "projectile_system.h"
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
    Entity *entity = prayEntityNew(C(CID_TRANSFORM, CID_PROJECTILE, CID_COLLIDER_2D), 3);
    TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
    transform->position = origin;
    transform->rotationDegrees = angle;
    ProjectileComponent *projectile = prayEntityGetComponent(entity, CID_PROJECTILE);
    projectile->origin = origin;
    projectile->speed = 4000;
    projectile->range = 4000;
    projectile->terminator = calculatePointOnCircle(origin, DEG2RAD * angle, projectile->range);
    projectile->damage = damage;
    Collider2DComponent *collider = prayEntityGetComponent(entity, CID_COLLIDER_2D);
    collider->type = COLLIDER_2D_CIRCLE;
    collider->radius = 40;
    prayEntityRegister(entity);
}

static void gameUpdate()
{
    LList projectiles;
    prayEntityLookupAll(&projectiles, C(CID_TRANSFORM, CID_PROJECTILE, CID_COLLIDER_2D), 3);
    if (projectiles.size == 0)
    {
        return;
    }

    LList targets;
    prayEntityLookupAll(&targets, C(CID_COLLIDER_2D, CID_TARGET, CID_TRANSFORM), 3);

    LNode *projectileNode = nullptr;
    LNode *targetNode = nullptr;
    LListForEach(&projectiles, projectileNode)
    {
        Entity *entity = LListGetEntry(projectileNode, Entity);
        TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
        ProjectileComponent *projectile = prayEntityGetComponent(entity, CID_PROJECTILE);
        Collider2DComponent *collider = prayEntityGetComponent(entity, CID_COLLIDER_2D);
        transform->position = prayVector2MoveTowards(transform->position,
                                                     projectile->terminator,
                                                     projectile->speed * GetFrameTime());

        LListForEach(&targets, targetNode)
        {
            Entity *targetEntity = LListGetEntry(targetNode, Entity);
            Collider2DComponent *targetCollider = prayEntityGetComponent(targetEntity, CID_COLLIDER_2D);
            TargetComponent *targetComponent = prayEntityGetComponent(targetEntity, CID_TARGET);
            TransformComponent *targetTransform = prayEntityGetComponent(targetEntity, CID_TRANSFORM);
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
        TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
        ProjectileComponent *projectile = prayEntityGetComponent(entity, CID_PROJECTILE);
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
