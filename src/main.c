
#include "astar.h"
#include "bool_mat.h"
#include "raylib.h"
#include "tmem.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
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
    float x = (random() % xMax) + xMin;
    float y = (random() % yMax) + yMin;
    return (Vector2) {x, y};
}

void printvec(Vector2 v)
{
    printf("Vec2 x=%.2f, y=%.2f\n", v.x, v.y);
}

constexpr int WORLD_HEIGHT = 50;
constexpr int WORLD_WIDTH = 50;

constexpr int TILE_SIZE = 30;

int main(void)
{
    const int screenWidth = 1500;
    const int screenHeight = 1500;

    tMemInit();

    InitWindow(screenWidth, screenHeight, "In the Name of Science!");

    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    SetWindowMinSize(screenWidth, screenHeight);

    BoolMat *navGrid = boolMatNew(WORLD_HEIGHT, WORLD_WIDTH, true, false);

    char world[WORLD_HEIGHT][WORLD_WIDTH] = {0};

    SetTargetFPS(60);

    Position startingPoint = {1, 1};

    while (!WindowShouldClose())
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

        BeginDrawing();

        ClearBackground(RAYWHITE);

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

        EndDrawing();
    }

    CloseWindow();

    navGrid = boolMatFree(navGrid);
    tMemDestroy();

    return 0;
}
