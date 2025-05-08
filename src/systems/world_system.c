
#include "world_system.h"
#include "bool_mat.h"
#include "itnos_components.h"
#include "pray_colors.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_system.h"
#include "raylib.h"
#include "world_component.h"
#include <pthread.h>
#include <threads.h>

static Entity *worldEntity;
static WorldComponent *world;

static void start()
{
    worldEntity = prayEntityNew(C(CID_WORLD), 1);
    world = prayEntityGetComponent(worldEntity, CID_WORLD);
    prayEntityRegister(worldEntity);
}

static void stop()
{
    prayEntityUnregister(worldEntity);
    prayEntityFree(worldEntity);
}

static void renderWorld()
{
    for (int i = 0; i < world->rows; i++)
    {
        for (int j = 0; j < world->cols; j++)
        {
            Color color;

            if (boolMatGet(world->navGrid, j, i) == 0)
            {
                color = BLACK;
            }
            else if (world->world[i][j] == '1')
            {
                color = RED;
            }
            else if (world->world[i][j] == '2')
            {
                color = GREEN;
            }
            else if (world->world[i][j] == '3')
            {
                color = BLUE;
            }
            else
            {
                if (i % 2 == 0)
                {
                    color = (j % 2 == 0) ? MATERIAL_BLUE_GREY_700 : MATERIAL_BLUE_GREY_800;
                }
                else
                {
                    color = (j % 2 == 0) ? MATERIAL_BLUE_GREY_800 : MATERIAL_BLUE_GREY_700;
                }
            }
            DrawRectangle(j * world->tileSize, 
                          i * world->tileSize, 
                          world->tileSize, 
                          world->tileSize, 
                          color);
        }
    }

    Rectangle rect;
    rect.height = world->tileSize * (float) world->rows;
    rect.width = world->tileSize * (float) world->cols;
    rect.x = 0;
    rect.y = 0;
    DrawRectangleLinesEx(rect, 10.0f, WHITE);
}

void registerWorldSystem()
{
    System worldSystem = {
        .name = "World System",
        .start = start,
        .stop = stop,
        .renderUpdateWorldSpace = renderWorld,
    };

    praySystemsRegister(worldSystem);
}
