
#include "pathfinding_system.h"
#include "astar.h"
#include "bool_mat.h"
#include "system.h"
#include "tmem.h"
#include <raylib.h>
#include <stdio.h>

constexpr static int WORLD_HEIGHT = 50;
constexpr static int WORLD_WIDTH = 50;
constexpr static int TILE_SIZE = 30;
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
    Vector2 mousePosition = GetMousePosition();
    int row = mousePosition.y / TILE_SIZE;
    int col = mousePosition.x / TILE_SIZE;

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        world[row][col] = 'X';
        boolMatSet(navGrid, col, row, false);
    }
    else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
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
                    color = (j % 2 == 0) ? WHITE : LIGHTGRAY;
                }
                else
                {
                    color = (j % 2 == 0) ? LIGHTGRAY : WHITE;
                }
            }
            DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
        }
    }
}

void registerPathingSystem()
{
    System *system = systemNew();
    snprintf(system->systemName, sizeof(system->systemName), "Pathfinding");
    system->start = start;
    system->close = close;
    system->gameUpdate = gameUpdate;
    system->renderUpdate = renderUpdate;
}
