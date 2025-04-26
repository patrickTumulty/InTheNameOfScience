
#include "pray_engine.h"
#include "pray_entity_registry.h"
#include "pray_camera.h"
#include "pray_globals.h"
#include "pray_system.h"
#include "tmem.h"

void prayEngineRun()
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "In the Name of Science!");

    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    SetWindowMinSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    SetTargetFPS(60);

    systemsRunStart();

    while (!WindowShouldClose())
    {
        systemsRunGameUpdate();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode2D(*getPrayCamera());

        DrawCircle(10, 10, 100, RED);

        systemsRunRenderUpdateWorldSpace();

        EndMode2D();

        systemsRunRenderUpdateScreenSpace();

        EndDrawing();
    }

    systemsRunStop();

    CloseWindow();

}


void prayEngineInitialize()
{
    tMemInit();
    entityRegistryInit();
    systemsInit();
}


void prayEngineDestroy()
{
    entityRegistryDestroy();
    systemsDestroy();
    tMemDestroy();
}

