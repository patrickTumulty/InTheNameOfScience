
#ifndef ITNOS_COMPONENTS_H
#define ITNOS_COMPONENTS_H

#include "bool_mat.h"
#include "common_types.h"
#include "pray_default_components.h"
#include "pray_entity.h"
#include "raylib.h"

#ifdef COMPONENT_OFFSET
#undef COMPONENT_OFFSET
#endif
#define COMPONENT_OFFSET COMPONENT_BANK_A

typedef struct
{
    u32 rows;
    u32 cols;
    float tileSize;
    BoolMat *navGrid;
    char **world;
} WorldComponent;

REGISTER_CID(WorldComponent);

typedef struct
{
    bool roaming;
    float t;
} Roam;

typedef struct
{
    Roam roam;
} UnitComponent;

REGISTER_CID(UnitComponent);

typedef struct
{
    Entity *target;
    int damage;
} TargetingComponent;

REGISTER_CID(TargetingComponent);

typedef struct
{
} EnemyComponent;

REGISTER_CID(EnemyComponent);

typedef struct
{
    bool selected;
} SelectableComponent;

REGISTER_CID(SelectableComponent);

typedef enum : u8
{
    COLLIDER_2D_BOX = 0,
    COLLIDER_2D_CIRCLE = 1,
    COLLIDER_2D_TRIANGLE = 2,
    COLLIDER_2D_POLY = 3,
} Collider2DType;

typedef struct
{
    Collider2DType type;
    float radius;
    Vector2 offset;
} Collider2DComponent;

REGISTER_CID(Collider2DComponent);

typedef void (*OnHealthDepletedCB)(void);

typedef struct
{
    OnHealthDepletedCB healthDepletedCallback;
    int maxHealth;
    int currentHealth;
} HealthComponent;

REGISTER_CID(HealthComponent);

typedef struct
{
    float damage;
    float roundsPerSecond;
    float radius;
    double lastFiredTimestamp;
    Entity *target;
} TurretComponent;

REGISTER_CID(TurretComponent);

typedef struct
{
    float speed;
    float damage;
    Vector2 origin;
    Vector2 terminator;
    float range;
    u8 targetBitmask;
} ProjectileComponent;

REGISTER_CID(ProjectileComponent);

typedef void (*OnHitCB)(Entity *targetEntity, Entity *projectileEntity, ProjectileComponent *projectileComponent);

typedef struct
{
    OnHitCB onHitCallback;
    u8 targetBitmask;
} TargetComponent;

REGISTER_CID(TargetComponent);

typedef struct
{
    LList path;
    Vector2 currentPoint;
    bool active;
    int index;
    float speed;
} PathfindComponent;

REGISTER_CID(PathfindComponent);

void registerComponents();

#endif // ITNOS_COMPONENTS_H
