
#include "unit_system.h"
#include "astar.h"
#include "common_types.h"
#include "common_utils.h"
#include "float.h"
#include "game_math.h"
#include "itnos_components.h"
#include "linked_list.h"
#include "pathfind_component.h"
#include "pointer_list.h"
#include "pray_camera.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_system.h"
#include "projectile_system.h"
#include "raylib.h"
#include "selection_system.h"
#include "tmem.h"
#include "world_component.h"
#include <stdatomic.h>
#include <threads.h>

static Texture2D textureBlue;
static Texture2D textureRed;
static Shader outlineShader;

#define VISION_CIRCLE 1000

static WorldComponent *world;

#define TEXTURE_ARROW_RED "assets/red-arrow3.png"
#define TEXTURE_ARROW_BLUE "assets/blue-arrow3.png"

#define POS2VEC(POS) \
    (Vector2) { .x = (POS).x, .y = (POS).y }
#define VEC2POS(VEC) \
    (Position) { .x = (VEC).x, .y = (VEC).y }

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

void calculateTriangle(Vector2 center, float roatationDegrees, float radius, Vector2 points[3])
{
    points[0] = calculatePointOnCircle(center, DEG2RAD * roatationDegrees, radius);
    points[1] = calculatePointOnCircle(center, DEG2RAD * (120 + roatationDegrees), radius);
    points[2] = calculatePointOnCircle(center, DEG2RAD * (240 + roatationDegrees), radius);
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
        CID_TARGETING,
        CID_PATHFINDING,
        CID_SPRITE_2D,
        CID_COLLIDER_2D,
        CID_SELECTABLE,
    };

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
    sprite2D->rotation = 90;
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
    // createEntity(2, 4, textureBlue, &outlineShader);
    // createEntity(2, 6, textureBlue, &outlineShader);
    // createEntity(2, 8, textureBlue, &outlineShader);
}

static void stop()
{
    UnloadTexture(textureBlue);
    UnloadTexture(textureRed);
    UnloadShader(outlineShader);
}

static void setPathForSelectedUnits(WorldComponent *world, Vector2 position)
{
    if (!selectionEntitiesSelected())
    {
        return;
    }

    PList selectedEntities;

    selectionGetSelectedEntities(&selectedEntities);

    for (int i = 0; i < selectedEntities.length; i++)
    {
        Entity *entity = plistGet(&selectedEntities, i);
        TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
        PathfindComponent *pathfind = prayEntityGetComponent(entity, CID_PATHFINDING);

        Position start = (Position) {
            .x = (int) (transform->position.x / world->tileSize),
            .y = (int) (transform->position.y / world->tileSize),
        };

        Position dest = (Position) {
            .x = (int) (position.x / world->tileSize),
            .y = (int) (position.y / world->tileSize),
        };

        pathfindClearPoints(pathfind);

        AStarPath path;

        astar(start, dest, world->navGrid, &path);

        if (path.pathLen <= 0)
        {
            tmemfree(path.path);
            continue;
        }

        float half = world->tileSize / 2;
        for (int i = 0; i < path.pathLen; i++)
        {
            Position p = path.path[i];
            Vector2 navPoint = {
                .x = ((float) p.x * world->tileSize) + half,
                .y = ((float) p.y * world->tileSize) + half,
            };
            pathfindAddPoint(pathfind, navPoint);
            world->world[p.y][p.x] = '3';
        }

        pathfindAddPoint(pathfind, position);

        tmemfree(path.path);
    }
}

static void renderUpdate()
{
    LList units;
    Rc rc = prayEntityLookupAll(&units, C(CID_TRANSFORM, CID_COLLIDER_2D), 2);

    LNode *node = nullptr;
    LListForEach(&units, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
        Collider2DComponent *collider2D = prayEntityGetComponent(entity, CID_COLLIDER_2D);

        Color green = GREEN;
        green.a = 120;

        DrawCircle((int) (transform->position.x + collider2D->offset.x),
                   (int) (transform->position.y + collider2D->offset.y),
                   collider2D->radius,
                   green);
    }

    
    rc = prayEntityLookupAll(&units, C(CID_TRANSFORM, CID_UNIT), 2);
    node = nullptr;
    LListForEach(&units, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);

        DrawCircleLines((int) transform->position.x, (int) transform->position.y, VISION_CIRCLE, YELLOW);
    }
}

static void clearAStarPath(WorldComponent *world)
{
    for (int i = 0; i < world->rows; i++)
    {
        for (int j = 0; j < world->cols; j++)
        {
            if (world->world[i][j] == '3' || world->world[i][j] == '2')
            {
                world->world[i][j] = 0;
            }
        }
    }
}

static void gameUpdate()
{
    int rows = (int) world->rows;
    int cols = (int) world->cols;

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        clearAStarPath(world);

        Vector2 position = GetScreenToWorld2D(GetMousePosition(), *prayGetCamera());
        int row = (int) (position.y / world->tileSize);
        int col = (int) (position.x / world->tileSize);

        if (inBounds(row, 0, rows) && inBounds(col, 0, cols))
        {
            setPathForSelectedUnits(world, position);
        }
    }

    if (IsKeyPressed(KEY_P))
    {
        Entity *entity = prayEntityLookup(C(CID_UNIT), 1);
        TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
        
        projectileNew(transform->position, transform->rotation, 10);
    }

    // LList enemies;
    // prayEntityLookupAll(&enemies, C(CID_ENEMY), 1);
    // LNode *enemyNode = nullptr;
    //
    // LList units;
    // prayEntityLookupAll(&units, C(CID_UNIT), 1);
    // LNode *unitNode = nullptr;
    //
    // LListForEach(&units, unitNode)
    // {
    //     Entity *unitEntity = LListGetEntry(unitNode, Entity);
    //     TransformComponent *unitTransform = prayEntityGetComponent(unitEntity, CID_TRANSFORM);
    //     TargetingComponent *unitTargeting = prayEntityGetComponent(unitEntity, CID_TARGETING);
    //     PathfindComponent *unitPathfind = prayEntityGetComponent(unitEntity, CID_PATHFINDING);
    //     
    //     if (unitTargeting->target != nullptr)
    //     {
    //         continue; // We already have a target
    //     }
    //
    //     LListForEach(&enemies, enemyNode)
    //     {
    //         Entity *enemyEntity = LListGetEntry(enemyNode, Entity);
    //         TransformComponent *enemyTransform = prayEntityGetComponent(enemyEntity, CID_TRANSFORM);
    //         Collider2DComponent *enemyCollider = prayEntityGetComponent(enemyEntity, CID_COLLIDER_2D);
    //
    //         if (CheckCollisionCircles(unitTransform->position, VISION_CIRCLE, enemyTransform->position, enemyCollider->radius))
    //         {
    //             pathfindClearPoints(unitPathfind);
    //             unitTransform->rotation = calculateAngle(enemyTransform->position, unitTransform->position);
    //             break;
    //         }
    //     }
    // }
}

void registerUnitSystem()
{
    System system = {
        .name = "Unit System",
        .start = start,
        .stop = stop,
        .gameUpdate = gameUpdate,
        .renderUpdateWorldSpace = renderUpdate,
    };

    praySystemsRegister(system);
}
