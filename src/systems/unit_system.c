
#include "unit_system.h"
#include "astar.h"
#include "common_types.h"
#include "common_utils.h"
#include "float.h"
#include "game_math.h"
#include "itnos_components.h"
#include "linked_list.h"
#include "pointer_list.h"
#include "pray_camera.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_globals.h"
#include "pray_system.h"
#include "pray_utils.h"
#include "raylib.h"
#include "selection_system.h"
#include "tmem.h"
#include <stdio.h>
#include <threads.h>

static Texture2D textureBlue;
static Texture2D textureRed;
static Shader outlineShader;


#define TEXTURE_ARROW_RED "assets/arrow-red2.png"
#define TEXTURE_ARROW_BLUE "assets/arrow-blue2.png"
#define POS2VEC(POS) \
    (Vector2) { .x = (POS).x, .y = (POS).y }
#define VEC2POS(VEC) \
    (Position) { .x = (VEC).x, .y = (VEC).y }

static void clearPath(AStarPath *path)
{
    if (path->path != nullptr)
    {
        tmemfree(path->path);
        path->path = nullptr;
        path->pathLen = 0;
    }
}


static void setOutlineShaderProperties(Shader *shader, Texture texture, bool enabled)
{
    float outlineSize = enabled ? 1.0f : 0.0f;
    float outlineColor[4] = {1.0f, 0.0f, 0.0f, 1.0f}; // Normalized RED color
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

bool isInCircle(Vector2 circleCenter, Vector2 p, float radius)
{
    float deltaX = p.x - circleCenter.x;
    float deltaY = p.y - circleCenter.y;
    return ((deltaX * deltaX) + (deltaY * deltaY)) < (radius * radius);
}

void calculateTrangle(Vector2 center, float roatationDegrees, float radius, Vector2 points[3])
{
    points[0] = calculateRotation(center, DEG2RAD * roatationDegrees, radius);
    points[1] = calculateRotation(center, DEG2RAD * (120 + roatationDegrees), radius);
    points[2] = calculateRotation(center, DEG2RAD * (240 + roatationDegrees), radius);
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
    };

    u32 cidsLen = sizeof(cids) / sizeof(ComponentID);

    Entity *unitEntity = prayEntityNew(cids, cidsLen);
    TransformComponent *transform = prayEntityGetComponent(unitEntity, CID_TRANSFORM);

    transform->position.x = x * TILE_SIZE;
    transform->position.y = y * TILE_SIZE;

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
    collider2D->radius = 30;

    prayEntityRegister(unitEntity);
}

static void start()
{
    textureBlue = LoadTexture(TEXTURE_ARROW_BLUE);
    textureRed = LoadTexture(TEXTURE_ARROW_RED);
    outlineShader = LoadShader(nullptr, TextFormat("shaders/glsl330/outline.fs", 330));

    setOutlineShaderProperties(&outlineShader, textureBlue, false);
    setOutlineShaderProperties(&outlineShader, textureRed, false);

    createEntity(2, 2, textureBlue, &outlineShader);
    createEntity(10, 10, textureRed, &outlineShader);
}

static void stop()
{
    UnloadTexture(textureBlue);
    UnloadTexture(textureRed);
    UnloadShader(outlineShader);
}

static void moveUnitAlongPath(TransformComponent *transform, PathfindComponent *pathfind)
{
    if (pathfind->pathSet == false)
    {
        return;
    }

    float half = TILE_SIZE / 2;

    Vector2 *position = &transform->position;
    Position temp = pathfind->path.path[pathfind->index];
    Vector2 pathPoint = {
        .x = (float) (temp.x * TILE_SIZE) + half,
        .y = (float) (temp.y * TILE_SIZE) + half,
    };

    *position = prayVector2MoveTowards(*position, pathPoint, pathfind->speed * GetFrameTime());

    if (isInCircle(pathPoint, *position, 1))
    {
        pathfind->index++;
        if (pathfind->index == pathfind->path.pathLen)
        {
            clearPath(&pathfind->path);
            pathfind->pathSet = false;
        }
        else
        {
            temp = pathfind->path.path[pathfind->index];
            pathPoint = (Vector2) {
                .x = (float) (temp.x * TILE_SIZE) + half,
                .y = (float) (temp.y * TILE_SIZE) + half,
            };
            float angle = calculateAngle(pathPoint, *position);
            transform->rotation = angle;
        }
    }
}

static void setPathForPathfindingUnits(WorldComponent *world, Vector2 position)
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
            .x = (int) transform->position.x / TILE_SIZE,
            .y = (int) transform->position.y / TILE_SIZE,
        };

        Position dest = (Position) {
            .x = (int) position.x / TILE_SIZE,
            .y = (int) position.y / TILE_SIZE,
        };

        clearPath(&pathfind->path);
        pathfind->index = 0;
        pathfind->pathSet = false;

        astar(start, dest, world->navGrid, &pathfind->path);

        if (pathfind->path.path != nullptr && pathfind->path.pathLen > 0)
        {
            pathfind->pathSet = true;

            for (int i = 0; i < pathfind->path.pathLen; i++)
            {
                Position p = pathfind->path.path[i];
                world->world[p.y][p.x] = '3';
            }
        }
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

        DrawCircle((int) transform->position.x,
                   (int) transform->position.y,
                   collider2D->radius,
                   green);
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
    Entity *worldEntity = prayEntityLookup(C(CID_WORLD), 1);
    WorldComponent *world = prayEntityGetComponent(worldEntity, CID_WORLD);

    int rows = (int) world->rows;
    int cols = (int) world->cols;

    LList units;
    Rc rc = prayEntityLookupAll(&units, C(CID_UNIT, CID_TRANSFORM, CID_PATHFINDING, CID_COLLIDER_2D, CID_SPRITE_2D), 3);
    if (rc != RC_OK)
    {
        return;
    }

    LNode *node = nullptr;
    LListForEach(&units, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
        PathfindComponent *pathfind = prayEntityGetComponent(entity, CID_PATHFINDING);

        moveUnitAlongPath(transform, pathfind);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        clearAStarPath(world);

        Vector2 position = GetScreenToWorld2D(GetMousePosition(), *prayGetCamera());
        int row = (int) position.y / TILE_SIZE;
        int col = (int) position.x / TILE_SIZE;

        if (inBounds(row, 0, rows) && inBounds(col, 0, cols))
        {
            setPathForPathfindingUnits(world, position);
        }
    }
}

void registerUnitSystem()
{
    System system = {
        .name = "Unit System",
        .start = start,
        .stop = stop,
        .gameUpdate = gameUpdate,
        // .renderUpdateWorldSpace = renderUpdate,
    };

    praySystemsRegister(system);
}
