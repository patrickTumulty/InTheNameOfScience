
#include "world_system.h"
#include "bool_mat.h"
#include "itnos_components.h"
#include "pray_colors.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_globals.h"
#include "pray_system.h"
#include <pthread.h>
#include <stdio.h>
#include <threads.h>

static Entity *worldEntity;
static World *worldComponent;

static void start()
{
    worldEntity = entityNew(C(CID_WORLD), 1);
    worldComponent = entityGetComponent(worldEntity, CID_WORLD);
    entityRegistryRegister(worldEntity);
}

static void stop()
{
    entityRegistryUnregister(worldEntity);
    entityFree(worldEntity);
}

static void renderWorld()
{
    for (int i = 0; i < worldComponent->rows; i++)
    {
        for (int j = 0; j < worldComponent->cols; j++)
        {
            Color color;

            if (boolMatGet(worldComponent->navGrid, j, i) == 0)
            {
                color = BLACK;
            }
            else if (worldComponent->world[i][j] == '1')
            {
                color = RED;
            }
            else if (worldComponent->world[i][j] == '2')
            {
                color = GREEN;
            }
            else if (worldComponent->world[i][j] == '3')
            {
                color = BLUE;
            }
            else
            {
                if (i % 2 == 0)
                {
                    color = (j % 2 == 0) ? MATERIAL_BLUE_GREY_100 : MATERIAL_BLUE_GREY_200;
                }
                else
                {
                    color = (j % 2 == 0) ? MATERIAL_BLUE_GREY_200 : MATERIAL_BLUE_GREY_100;
                }
            }
            DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
        }
    }

    Rectangle rect;
    rect.height = TILE_SIZE * worldComponent->rows;
    rect.width = TILE_SIZE * worldComponent->cols;
    rect.x = 0;
    rect.y = 0;
    DrawRectangleLinesEx(rect, 3.0f, MATERIAL_BLUE_GREY_700);
}

void registerWorldSystem()
{
    System worldSystem = {
        .systemName = "World System",
        .start = start,
        .stop = stop,
        .renderUpdateWorldSpace = renderWorld,
    };

    praySystemsRegister(worldSystem);
}
