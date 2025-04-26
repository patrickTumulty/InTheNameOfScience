
#ifndef TEST_COMPONENTS_H
#define TEST_COMPONENTS_H

#include "common_types.h"
#include "raylib.h"

typedef enum : u32
{
    EMPTY = 0,
    PLAYER = 1,
    TRANSFORM = 2,
    HEALTH = 3,
    WORLD = 4,
} TestComponentID;

typedef struct
{
    char playerName[20];
    int playerAge;
    char origin[20];
} PlayerComponent;

typedef struct
{
    Vector2 position;
} TransformComponent;

typedef struct
{
    u32 currentHealth;
    u32 maxHealth;
} HealthComponent;

typedef struct
{
    u8 *world;
    u64 worldSize;
} WorldComponent;

void registerTestComponents();


#endif // TEST_COMPONENTS_H
