
#include "pathfinding_system.h"
#include "astar.h"
#include "bool_mat.h"
#include "camera_system.h"
#include "common_types.h"
#include "entity_registry.h"
#include "linked_list.h"
#include "pray_globals.h"
#include "system.h"
#include "tmem.h"
#include <raylib.h>
#include <stdio.h>


static void gameUpdate()
{
    ComponentID cids[] = {CID_TRANSFORM, CID_WORLD};
    Entity *worldEntity = entityRegistryLookupFirst(cids, 2);
    World *worldComponent = entityGetComponent(worldEntity, CID_WORLD);

    Position p = {1, 1};

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 position = GetScreenToWorld2D(GetMousePosition(), getCamera());
        int row = (int) position.y / TILE_SIZE;
        int col = (int) position.x / TILE_SIZE;

        worldComponent->world[row][col] = 'X';
        boolMatSet(worldComponent->navGrid, col, row, false);
    }
    else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 position = GetScreenToWorld2D(GetMousePosition(), getCamera());
        int row = (int) position.y / TILE_SIZE;
        int col = (int) position.x / TILE_SIZE;

        // Clear path
        for (int i = 0; i < WORLD_HEIGHT; i++)
        {
            for (int j = 0; j < WORLD_WIDTH; j++)
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

    worldComponent->world[p.y][p.x] = '1';
}

void registerPathingSystem()
{
    System *system = systemNew();
    snprintf(system->systemName, sizeof(system->systemName), "Pathfinding");
    // system->start = start;
    // system->stop = close;
    system->gameUpdate = gameUpdate;
    // system->renderUpdateWorldSpace = renderUpdate;
}
