
#include "pathfinding_system.h"
#include "astar.h"
#include "bool_mat.h"
#include "common_types.h"
#include "common_utils.h"
#include "itnos_components.h"
#include "pray_camera.h"
#include "pray_entity_registry.h"
#include "pray_globals.h"
#include "pray_system.h"
#include "tmem.h"
#include <raylib.h>
#include <stdio.h>


static void gameUpdate()
{
    Entity *worldEntity = entityRegistryLookupFirst(C(CID_WORLD), 1);
    WorldComponent *worldComponent = entityGetComponent(worldEntity, CID_WORLD);

    Position p = {1, 1};

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 position = GetScreenToWorld2D(GetMousePosition(), *getPrayCamera());
        int row = (int) position.y / TILE_SIZE;
        int col = (int) position.x / TILE_SIZE;

        if (inBounds(row, 0, (int) worldComponent->rows) && inBounds(col, 0, (int) worldComponent->cols))
        {
            worldComponent->world[row][col] = 'X';
            boolMatSet(worldComponent->navGrid, col, row, false);
        }
    }
    else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 position = GetScreenToWorld2D(GetMousePosition(), *getPrayCamera());
        int row = (int) position.y / TILE_SIZE;
        int col = (int) position.x / TILE_SIZE;

        if (inBounds(row, 0, (int) worldComponent->rows) && inBounds(col, 0, (int) worldComponent->cols))
        {
            // Clear path
            for (int i = 0; i < worldComponent->rows; i++)
            {
                for (int j = 0; j < worldComponent->cols; j++)
                {
                    if (worldComponent->world[i][j] == '3' || worldComponent->world[i][j] == '2')
                    {
                        worldComponent->world[i][j] = 0;
                    }
                }
            }

            AStarPath path = {};

            astar(p, (Position) {.x = col, .y = row}, worldComponent->navGrid, &path);

            if (path.path != NULL || path.pathLen != 0)
            {
                for (int i = 0; i < path.pathLen; i++)
                {
                    Position p = path.path[i];
                    worldComponent->world[p.y][p.x] = '3';
                }
                tmemfree(path.path);
            }

            worldComponent->world[row][col] = '2';
        }
    }

    worldComponent->world[p.y][p.x] = '1';
}

void registerPathingSystem()
{
    System system = {
        .systemName = "Pathfinding",
        .gameUpdate = gameUpdate,
    };

    praySystemsRegister(system);
}
