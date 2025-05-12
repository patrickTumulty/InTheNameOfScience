
#include "turret_system.h"
#include "game_math.h"
#include "itnos_components.h"
#include "linked_list.h"
#include "pray_default_components.h"
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
    prayEntityLookupAll(&turrets, C(CID(TurretComponent)), 1);

    LList targets;
    prayEntityLookupAll(&targets, C(CID(TargetComponent)), 1);

    LNode *node1 = nullptr;
    LNode *node2 = nullptr;
    LListForEach(&turrets, node1)
    {
        Entity *turretEntity = LListGetEntry(node1, Entity);
        TurretComponent *turret = getComponent(turretEntity, TurretComponent);
        Transform2DComponent *turretTransform = getComponent(turretEntity, Transform2DComponent);

        if (turret->target != nullptr)
        {
            Transform2DComponent *targetTransform = getComponent(turret->target, Transform2DComponent);

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
            Transform2DComponent *targetTransform = getComponent(targetEntity, Transform2DComponent);

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
    prayEntityLookupAll(&turrets, C(CID(TurretComponent)), 1);

    LNode *node1 = nullptr;
    LListForEach(&turrets, node1)
    {
        Entity *turretEntity = LListGetEntry(node1, Entity);
        TurretComponent *turret = getComponent(turretEntity, TurretComponent);
        Transform2DComponent *turretTransform = getComponent(turretEntity, Transform2DComponent);

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
