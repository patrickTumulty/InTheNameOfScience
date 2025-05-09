
#ifndef PROJECTILE_COMPONENT_H
#define PROJECTILE_COMPONENT_H

#include "raylib.h"
typedef struct {
    float speed;
    float damage;
    Vector2 origin;
    Vector2 terminator;
    float range;
} ProjectileComponent;

#endif // PROJECTILE_COMPONENT_H
