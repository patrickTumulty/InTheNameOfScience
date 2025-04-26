
#include "pathfinding_system.h"
#include "astar.h"
#include "bool_mat.h"
#include "camera_system.h"
#include "pray_colors.h"
#include "pray_globals.h"
#include "system.h"
#include "tmem.h"
#include <raylib.h>
#include <stdio.h>

static BoolMat *navGrid;
static char world[WORLD_HEIGHT][WORLD_WIDTH] = {0};
static Position startingPoint = {1, 1};

static void start()
{
    navGrid = boolMatNew(WORLD_HEIGHT, WORLD_WIDTH, true, false);
}

static void close()
{
    navGrid = boolMatFree(navGrid);
}

static void gameUpdate()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 position = GetScreenToWorld2D(GetMousePosition(), getCamera());
        int row = (int) position.y / TILE_SIZE;
        int col = (int) position.x / TILE_SIZE;

        world[row][col] = 'X';
        boolMatSet(navGrid, col, row, false);
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
                if (world[i][j] == '3' || world[i][j] == '2')
                {
                    world[i][j] = 0;
                }
            }
        }

        AStarPath path = {};

        astar(startingPoint, (Position) {.x = col, .y = row}, navGrid, &path);

        if (path.path != NULL || path.pathLen != 0)
        {
            for (int i = 0; i < path.pathLen; i++)
            {
                Position p = path.path[i];
                world[p.y][p.x] = '3';
            }
            tmemfree(path.path);
        }

        world[row][col] = '2';
    }

    world[startingPoint.y][startingPoint.x] = '1';
}

static void renderUpdate()
{
    for (int i = 0; i < WORLD_HEIGHT; i++)
    {
        for (int j = 0; j < WORLD_WIDTH; j++)
        {
            Color color;

            if (boolMatGet(navGrid, j, i) == 0)
            {
                color = BLACK;
            }
            else if (world[i][j] == '1')
            {
                color = RED;
            }
            else if (world[i][j] == '2')
            {
                color = GREEN;
            }
            else if (world[i][j] == '3')
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
    rect.height = TILE_SIZE * WORLD_HEIGHT;
    rect.width = TILE_SIZE * WORLD_WIDTH;
    rect.x = 0;
    rect.y = 0;
    DrawRectangleLinesEx(rect, 3.0f, MATERIAL_BLUE_GREY_700);
}

void registerPathingSystem()
{
    System *system = systemNew();
    snprintf(system->systemName, sizeof(system->systemName), "Pathfinding");
    system->start = start;
    system->stop = close;
    system->gameUpdate = gameUpdate;
    system->renderUpdateWorldSpace = renderUpdate;
}
