
#ifndef PROJECTILE_COMPONENT_H
#define PROJECTILE_COMPONENT_H

#include "common_types.h"
#include "pray_entity.h"
#include "raylib.h"


typedef struct {
    float speed;
    float damage;
    Vector2 origin;
    Vector2 terminator;
    float range;
    u8 targetBitmask;
} ProjectileComponent;

typedef void(*OnHitCB)(Entity *targetEntity, Entity *projectileEntity, ProjectileComponent *projectileComponent);

typedef struct {
    OnHitCB onHitCallback;
    u8 targetBitmask;
} TargetComponent;

#endif // PROJECTILE_COMPONENT_H
