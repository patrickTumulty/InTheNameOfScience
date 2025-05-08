
#ifndef ITNOS_COMPONENTS_H
#define ITNOS_COMPONENTS_H

#include "astar.h"
#include "bool_mat.h"
#include "common_types.h"
#include "pray_entity.h"
#include "raylib.h"

struct Sprite2DComponent;

typedef void (*PreShaderCallback)(Entity *entity, struct Sprite2DComponent *sprite2D);

typedef struct {
    bool roaming;
    float t;
} Roam;

typedef struct
{
    Roam roam;
} UnitComponent;

typedef struct
{
    int maxHealth;
    int currentHealth;
} HealthComponent;

typedef struct
{
    int damage;
} DamageComponent;

typedef struct Sprite2DComponent
{
    Texture2D texture;
    Rectangle source;
    Vector2 origin;
    float rotation; // degrees
    Shader *shader;
    PreShaderCallback shaderCallback;
} Sprite2DComponent;

typedef struct
{
    bool selected;
} SelectableComponent;

typedef struct
{
    Vector2 position;
    float rotation; // 0.0 to 359.0
} TransformComponent;

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

typedef enum : u32
{
    CID_RESERVED = 0,
    CID_WORLD,
    CID_TRANSFORM,
    CID_UNIT,
    CID_PATHFINDING,
    CID_SPRITE_2D,
    CID_COLLIDER_2D,
    CID_SELECTABLE,
    CID_HEALTH,
    CID_DAMAGE,
    CID_ENEMY,
} ComponentID;

#define C(...) \
    (ComponentID[]) { __VA_ARGS__ }


char *componentID2Str(ComponentID id);

void registerComponents();

#endif // ITNOS_COMPONENTS_H
