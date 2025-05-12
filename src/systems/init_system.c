
#include "init_system.h"
#include "itnos_components.h"
#include "pray_component.h"
#include "pray_default_components.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_system.h"
#include <float.h>
#include <inttypes.h>
#include <stdio.h>

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

    SelectableComponent *selectable = getComponent(entity, SelectableComponent);
    if (selectable == nullptr)
    {
        return;
    }

    setOutlineShaderProperties(sprite2D->shader, sprite2D->texture, selectable->selected);
}


static void createEntity(float x, float y, Texture2D texture, Shader *shader)
{
    cid cids[] = {
        CID(UnitComponent),
        CID(Transform2DComponent),
        CID(PathfindComponent),
        CID(Sprite2DComponent),
        CID(Collider2DComponent),
        CID(SelectableComponent),
        CID(TargetComponent),
    };

    u32 cidsLen = sizeof(cids) / sizeof(cid);

    Entity *unitEntity = prayEntityNew(cids, cidsLen);

    auto transform = getComponent(unitEntity, Transform2DComponent);

    transform->position.x = x * world->tileSize;
    transform->position.y = y * world->tileSize;

    float textureWidth = (float) texture.width;
    float textureHeight = (float) texture.height;

    auto sprite2D = getComponent(unitEntity, Sprite2DComponent);

    sprite2D->texture = texture;
    sprite2D->source = (Rectangle) {0, 0, textureWidth, textureHeight};
    sprite2D->origin = (Vector2) {textureWidth / 2, textureHeight / 2};
    sprite2D->rotationDegrees = 90;
    sprite2D->shader = shader;
    sprite2D->preShaderCallback = configureShader;

    auto collider2D = getComponent(unitEntity, Collider2DComponent);

    collider2D->type = COLLIDER_2D_TRIANGLE;
    collider2D->radius = 80;
    collider2D->offset = (Vector2) {.x = -20, .y = 0};

    prayEntityRegister(unitEntity);
}

#define LIST(...) {CID(__VA_ARGS__)}

static void createTargetEntity()
{
    Entity *entity = prayEntityNew(C(CID(EnemyComponent),
                                     CID(HealthComponent),
                                     CID(Transform2DComponent),
                                     CID(Collider2DComponent)),
                                   4);
    auto transform = getComponent(entity, Transform2DComponent);
    transform->position.x = 20 * world->tileSize;
    transform->position.y = 20 * world->tileSize;

    auto collider = getComponent(entity, Collider2DComponent);
    collider->radius = 100;

    prayEntityRegister(entity);
}

static void createTurret()
{
    cid cids[] = {
        CID(TurretComponent),
        CID(Transform2DComponent),
    };

    u32 cidsLen = sizeof(cids) / sizeof(cid);

    Entity *turretEntity = prayEntityNew(cids, cidsLen);
    auto turret = getComponent(turretEntity, TurretComponent);
    turret->radius = 2000;
    turret->roundsPerSecond = 0.25f;
    auto transform = getComponent(turretEntity, Transform2DComponent);
    transform->position.x = 256 * 25;
    transform->position.y = 256 * 25;

    prayEntityRegister(turretEntity);
}

static void start()
{
    Entity *worldEntity = prayEntityLookup(C(CID(WorldComponent)), 1);
    world = getComponent(worldEntity, WorldComponent);

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
