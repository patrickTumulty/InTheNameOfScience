
#include "init_system.h"
#include "itnos_components.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_system.h"
#include "turret_component.h"
#include "turret_system.h"
#include "world_component.h"
#include <float.h>

#define TEXTURE_ARROW_RED "assets/red-arrow3.png"
#define TEXTURE_ARROW_BLUE "assets/blue-arrow3.png"

static WorldComponent *world;
static Texture2D textureBlue;
static Texture2D textureRed;
static Shader outlineShader;

static void setOutlineShaderProperties(Shader *shader, Texture texture, bool enabled)
{
    float outlineSize = enabled ? 6.0f : 0.0f;
    float outlineColor[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // Normalized WHITE color
    float textureSize[2] = {(float) texture.width, (float) texture.height};

    // Get shader locations
    int outlineSizeLoc = GetShaderLocation(*shader, "outlineSize");
    int outlineColorLoc = GetShaderLocation(*shader, "outlineColor");
    int textureSizeLoc = GetShaderLocation(*shader, "textureSize");

    // Set shader values (they can be changed later)
    SetShaderValue(*shader, outlineSizeLoc, &outlineSize, SHADER_UNIFORM_FLOAT);
    SetShaderValue(*shader, outlineColorLoc, outlineColor, SHADER_UNIFORM_VEC4);
    SetShaderValue(*shader, textureSizeLoc, textureSize, SHADER_UNIFORM_VEC2);
}

static void configureShader(Entity *entity, struct Sprite2DComponent *sprite2D)
{
    if (sprite2D->shader == nullptr)
    {
        return;
    }

    SelectableComponent *selectable = prayEntityGetComponent(entity, CID_SELECTABLE);
    if (selectable == nullptr)
    {
        return;
    }

    setOutlineShaderProperties(sprite2D->shader, sprite2D->texture, selectable->selected);
}


static void createEntity(float x, float y, Texture2D texture, Shader *shader)
{
    ComponentID cids[] = {
        CID_UNIT,
        CID_TRANSFORM,
        CID_PATHFINDING,
        CID_SPRITE_2D,
        CID_COLLIDER_2D,
        CID_SELECTABLE,
        CID_TARGET};

    u32 cidsLen = sizeof(cids) / sizeof(ComponentID);

    Entity *unitEntity = prayEntityNew(cids, cidsLen);
    TransformComponent *transform = prayEntityGetComponent(unitEntity, CID_TRANSFORM);

    transform->position.x = x * world->tileSize;
    transform->position.y = y * world->tileSize;

    float textureWidth = (float) texture.width;
    float textureHeight = (float) texture.height;

    Sprite2DComponent *sprite2D = prayEntityGetComponent(unitEntity, CID_SPRITE_2D);

    sprite2D->texture = texture;
    sprite2D->source = (Rectangle) {0, 0, textureWidth, textureHeight};
    sprite2D->origin = (Vector2) {textureWidth / 2, textureHeight / 2};
    sprite2D->rotationDegrees = 90;
    sprite2D->shader = shader;
    sprite2D->shaderCallback = configureShader;

    Collider2DComponent *collider2D = prayEntityGetComponent(unitEntity, CID_COLLIDER_2D);
    collider2D->type = COLLIDER_2D_TRIANGLE;
    collider2D->radius = 80;
    collider2D->offset = (Vector2) {.x = -20, .y = 0};

    prayEntityRegister(unitEntity);
}

static void createTargetEntity()
{
    Entity *entity = prayEntityNew(C(CID_ENEMY, CID_HEALTH, CID_TRANSFORM, CID_COLLIDER_2D), 4);
    TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
    transform->position.x = 20 * world->tileSize;
    transform->position.y = 20 * world->tileSize;

    Collider2DComponent *collider = prayEntityGetComponent(entity, CID_COLLIDER_2D);
    collider->radius = 100;

    prayEntityRegister(entity);
}

static void createTurret()
{
    ComponentID cids[] = {
        CID_TURRET,
        CID_TRANSFORM,
    };

    u32 cidsLen = sizeof(cids) / sizeof(ComponentID);

    Entity *turretEntity = prayEntityNew(cids, cidsLen);
    TurretComponent *turret = prayEntityGetComponent(turretEntity, CID_TURRET);
    turret->radius = 2000;
    turret->roundsPerSecond = 0.25f;
    TransformComponent *transform = prayEntityGetComponent(turretEntity, CID_TRANSFORM);
    transform->position.x = 256 * 25;
    transform->position.y = 256 * 25;

    prayEntityRegister(turretEntity);
}

static void start()
{

    Entity *worldEntity = prayEntityLookup(C(CID_WORLD), 1);
    world = prayEntityGetComponent(worldEntity, CID_WORLD);

    textureBlue = LoadTexture(TEXTURE_ARROW_BLUE);
    textureRed = LoadTexture(TEXTURE_ARROW_RED);
    outlineShader = LoadShader(nullptr, TextFormat("shaders/glsl330/outline.fs", 330));

    setOutlineShaderProperties(&outlineShader, textureBlue, false);
    setOutlineShaderProperties(&outlineShader, textureRed, false);

    // createTargetEntity();

    createEntity(2, 2, textureBlue, &outlineShader);
    createEntity(2, 4, textureBlue, &outlineShader);
    createEntity(2, 6, textureBlue, &outlineShader);
    // createEntity(2, 8, textureBlue, &outlineShader);

    createTurret();
}

static void stop()
{

    UnloadTexture(textureBlue);
    UnloadTexture(textureRed);
    UnloadShader(outlineShader);
}

void registerInitSystem()
{
    System system = {
        .name = "Init",
        .start = start,
        .stop = stop,
    };
    praySystemsRegister(system);
}
