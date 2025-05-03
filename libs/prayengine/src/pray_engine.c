
#include "pray_engine.h"
#include "pray_camera.h"
#include "pray_entity_registry.h"
#include "pray_globals.h"
#include "pray_system.h"
#include "raylib.h"
#include "tmem.h"

void prayEngineRun()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "In the Name of Science!");

    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    SetWindowMinSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetExitKey(KEY_NULL);

    SetTargetFPS(60);

    praySystemsRunStart();

    while (!WindowShouldClose())
    {
        praySystemsRunGameUpdate();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode2D(*prayGetCamera());

        praySystemsRunRenderUpdateWorldSpace();

        EndMode2D();

        praySystemsRunRenderUpdateScreenSpace();

        EndDrawing();
    }

    praySystemsRunStop();

    CloseWindow();
}

void prayEngineInitialize()
{
    tMemInit();
    praySystemsInit();
    prayEntityRegistryInit();
}

void prayEngineDestroy()
{
    prayEntityRegistryDestroy();
    praySystemsDestroy();
    tMemDestroy();
}
