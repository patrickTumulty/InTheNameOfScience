
#include "camera_system.h"
#include "entity_registry.h"
#include "pathfinding_system.h"
#include "pray_globals.h"
#include "raylib.h"
#include "system.h"
#include "tmem.h"
#include "world_system.h"
#include <math.h>
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

int main(void)
{
    tMemInit();
    entityRegistryInit();
    systemsInit();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "In the Name of Science!");

    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    SetWindowMinSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    SetTargetFPS(60);

    registerWorldSystem();
    registerCameraSystem();
    registerPathingSystem();

    systemsRunStart();

    while (!WindowShouldClose())
    {
        systemsRunGameUpdate();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode2D(getCamera());

        DrawCircle(10, 10, 100, RED);

        systemsRunRenderUpdateWorldSpace();

        EndMode2D();

        systemsRunRenderUpdateScreenSpace();

        EndDrawing();
    }

    systemsRunStop();

    CloseWindow();

    entityRegistryDestroy();
    systemsDestroy();
    tMemDestroy();

    return 0;
}
