
#include "turret_system.h"
#include "game_math.h"
#include "itnos_components.h"
#include "linked_list.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_system.h"
#include "projectile_system.h"
#include "raylib.h"
#include "turret_component.h"
#include <stdio.h>

static void gameUpdate()
{
    LList turrets;
    prayEntityLookupAll(&turrets, C(CID_TURRET), 1);

    LList targets;
    prayEntityLookupAll(&targets, C(CID_TARGET), 1);

    LNode *node1 = nullptr;
    LNode *node2 = nullptr;
    LListForEach(&turrets, node1)
    {
        Entity *turretEntity = LListGetEntry(node1, Entity);
        TurretComponent *turret = prayEntityGetComponent(turretEntity, CID_TURRET);
        TransformComponent *turretTransform = prayEntityGetComponent(turretEntity, CID_TRANSFORM);

        if (turret->target != nullptr)
        {
            TransformComponent *targetTransform = prayEntityGetComponent(turret->target, CID_TRANSFORM);

            float distance = vector2Distance(turretTransform->position, targetTransform->position);
            if (distance > turret->radius)
            {
                turret->target = nullptr;
            }
            else
            {
                turretTransform->rotationDegrees = calculateAngle(targetTransform->position, turretTransform->position);
                if (turret->lastFiredTimestamp < GetTime())
                {
                    projectileNew(turretTransform->position, turretTransform->rotationDegrees, 10);
                    turret->lastFiredTimestamp = GetTime() + turret->roundsPerSecond;
                }
            }
        }

        if (turret->target != nullptr)
        {
            continue;
        }

        LListForEach(&targets, node2)
        {
            Entity *targetEntity = LListGetEntry(node2, Entity);
            TransformComponent *targetTransform = prayEntityGetComponent(targetEntity, CID_TRANSFORM);

            float distance = vector2Distance(turretTransform->position, targetTransform->position);
            if (distance < turret->radius)
            {
                turret->target = targetEntity;
                continue;
            }
        }
    }
}

static void renderUpdate()
{
    LList turrets;
    prayEntityLookupAll(&turrets, C(CID_TURRET), 1);

    LNode *node1 = nullptr;
    LListForEach(&turrets, node1)
    {
        Entity *turretEntity = LListGetEntry(node1, Entity);
        TurretComponent *turret = prayEntityGetComponent(turretEntity, CID_TURRET);
        TransformComponent *turretTransform = prayEntityGetComponent(turretEntity, CID_TRANSFORM);

        int x = (int) turretTransform->position.x;
        int y = (int) turretTransform->position.y;
        DrawCircle(x, y, 150, RED);

        DrawCircleLines(x, y, turret->radius, RED);
    }
}

void registerTurretSystem()
{
    System system = {
        .name = "Turret",
        .gameUpdate = gameUpdate,
        .renderUpdateWorldSpace = renderUpdate,
    };
    praySystemsRegister(system);
}
