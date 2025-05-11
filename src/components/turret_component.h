
#ifndef TURRET_COMPONENT_H
#define TURRET_COMPONENT_H

#include "pray_entity.h"

typedef struct {
    float damage;
    float roundsPerSecond;
    float radius;
    double lastFiredTimestamp;
    Entity *target;
} TurretComponent;

#endif // TURRET_COMPONENT_H
