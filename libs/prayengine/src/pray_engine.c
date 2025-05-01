
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

    praySystemsRunStart();

    while (!WindowShouldClose())
    {
        praySystemsRunGameUpdate();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode2D(*getPrayCamera());

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
    entityRegistryInit();
}


void prayEngineDestroy()
{
    entityRegistryDestroy();
    praySystemsDestroy();
    tMemDestroy();
}

