
#include "array_list.h"
#include "entity_registry.h"
#include "pathfinding_system.h"
#include "raylib.h"
#include "system.h"
#include "tmem.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

float sinwave(float frequency, float phase, float ampliture)
{
    return (sin(2 * PI * frequency * GetTime() + phase) * ampliture) * GetFrameTime();
}

typedef struct
{
    Color color;
    Vector2 position;
    float speed;
} Unit;


Vector2 getRandomPosition(int xMin, int xMax, int yMin, int yMax)
{
    float x = (float) ((random() % xMax) + xMin);
    float y = (float) ((random() % yMax) + yMin);
    return (Vector2) {x, y};
}

void printvec(Vector2 v)
{
    printf("Vec2 x=%.2f, y=%.2f\n", v.x, v.y);
}

static Vector2 cameraTarget = {200, 200};
static Camera2D camera = {0};
static float cameraSpeed = 200.0f;

const static int WORLD_HEIGHT = 50;
const static int WORLD_WIDTH = 50;

constexpr static int TILE_SIZE = 30;

const static int SCREEN_WIDTH = 1500;
const static int SCREEN_HEIGHT = 1500;

Vector2 getScreenCenter()
{
    return (Vector2) {(float) SCREEN_WIDTH / 2, (float) SCREEN_HEIGHT / 2};
}

void gameStart()
{
    cameraTarget = getScreenCenter();
    camera.target = cameraTarget;
    camera.offset = (Vector2) {0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.125f;
}


void gameUpdate()
{
    cameraTarget.x -= (float) IsKeyDown(KEY_A) * (1.0f * GetFrameTime() * cameraSpeed);
    cameraTarget.x += (float) IsKeyDown(KEY_D) * (1.0f * GetFrameTime() * cameraSpeed);
    cameraTarget.y -= (float) IsKeyDown(KEY_W) * (1.0f * GetFrameTime() * cameraSpeed);
    cameraTarget.y += (float) IsKeyDown(KEY_S) * (1.0f * GetFrameTime() * cameraSpeed);

    float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
        const float zoomIncrement = 0.125f;
        camera.zoom += (wheel * zoomIncrement);
        if (camera.zoom < 1.0f)
        {
            camera.zoom = 1.0f;
        }

        if (camera.zoom > 8.0f)
        {
            camera.zoom = 8.0f;
        }
    }

    camera.target = cameraTarget;
}


const static Color MATERIAL_BLUE_GREY_700 = (Color) {69, 90, 100, 255};
const static Color MATERIAL_BLUE_GREY_100 = (Color) {207, 216, 220, 255};
const static Color MATERIAL_BLUE_GREY_200 = (Color) {176, 190, 197, 255};

void gameRender()
{
    // BeginMode2D(camera);
    //
    // float width = (float) GetScreenWidth();
    // float height = (float) GetScreenHeight();
    // camera.offset = (Vector2) {width / 2, height / 2};
    //
    //
    // for (int i = 0; i < WORLD_HEIGHT; i++)
    // {
    //     for (int j = 0; j < WORLD_WIDTH; j++)
    //     {
    //
    //
    //         Color color;
    //         if (i % 2 == 0)
    //         {
    //             color = (j % 2 == 0) ? MATERIAL_BLUE_GREY_100 : MATERIAL_BLUE_GREY_200;
    //         }
    //         else
    //         {
    //             color = (j % 2 == 0) ? MATERIAL_BLUE_GREY_200 : MATERIAL_BLUE_GREY_100;
    //         }
    //         DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
    //     }
    // }
    //
    // Rectangle rect;
    // rect.height = TILE_SIZE * WORLD_HEIGHT;
    // rect.width = TILE_SIZE * WORLD_WIDTH;
    // rect.x = 0;
    // rect.y = 0;
    //
    // DrawRectangleLinesEx(rect, 3.0f, MATERIAL_BLUE_GREY_700);
    //
    // EndMode2D();
}

void gameShutdown()
{
}

int main(void)
{
    tMemInit();
    entityRegistryInit();
    systemsInit();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "In the Name of Science!");

    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    SetWindowMinSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    SetTargetFPS(60);

    registerPathingSystem();

    AList *systems = systemsGet();
    for (int i = 0; i < systems->length; i++)
    {
        System *system = alistGet(systems, i);
        system->start();
    }

    // gameStart();

    while (!WindowShouldClose())
    {
        // gameUpdate();

        for (int i = 0; i < systems->length; i++)
        {
            System *system = alistGet(systems, i);
            system->gameUpdate();
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < systems->length; i++)
        {
            System *system = alistGet(systems, i);
            system->renderUpdate();
        }

        // gameRender();

        EndDrawing();
    }

    gameShutdown();

    for (int i = 0; i < systems->length; i++)
    {
        System *system = alistGet(systems, i);
        system->close();
    }

    CloseWindow();

    entityRegistryDestroy();
    systemsDestroy();
    tMemDestroy();

    return 0;
}
