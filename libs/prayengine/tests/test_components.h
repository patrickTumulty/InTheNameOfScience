
#ifndef TEST_COMPONENTS_H
#define TEST_COMPONENTS_H

#include "common_types.h"
#include "pray_component.h"
#include "raylib.h"

#define COMPONENT_OFFSET COMPONENT_BANK_A

typedef struct
{
    char playerName[20];
    int playerAge;
    char origin[20];
} PlayerComponent;

REGISTER_CID(PlayerComponent);

typedef struct
{
    char enemyName[20];
    int enemyAge;
    char origin[20];
} EnemyComponent;

REGISTER_CID(EnemyComponent);

typedef struct
{
    Vector2 position;
} TransformComponent;

REGISTER_CID(TransformComponent);

typedef struct
{
    u32 currentHealth;
    u32 maxHealth;
} HealthComponent;

REGISTER_CID(HealthComponent);

typedef struct
{
    u8 *world;
    u64 worldSize;
} WorldComponent;

REGISTER_CID(WorldComponent);

void registerTestComponents();


#endif // TEST_COMPONENTS_H
