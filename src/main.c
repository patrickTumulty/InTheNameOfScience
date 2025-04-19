
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

#define MIN(V1, V2) ((V1) < (V2) ? (V1) : (V2))
#define MAX(V1, V2) ((V1) > (V2) ? (V1) : (V2))
#define CLAMP(VAL, MIN_VAL, MAX_VAL) (MIN((MAX_VAL), MAX((MIN_VAL), (VAL))))

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

constexpr int WORLD_HEIGHT = 100;
constexpr int WORLD_WIDTH = 100;

constexpr int TILE_SIZE = 30;

int main(void)
{
    const int screenWidth = 1500;
    const int screenHeight = 1500;

    InitWindow(screenWidth, screenHeight, "In the Name of Science!");

    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    SetWindowMinSize(screenWidth, screenHeight);

    char world[WORLD_HEIGHT][WORLD_WIDTH] = {0};

    SetTargetFPS(60);

    Vector2 startingPoint = {10, 10};

    world[10][10] = '1';

    while (!WindowShouldClose())
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 mousePosition = GetMousePosition();
            int row = mousePosition.y / TILE_SIZE;
            int col = mousePosition.x / TILE_SIZE;
            world[row][col] = 'X';
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < WORLD_HEIGHT; i++)
        {
            for (int j = 0; j < WORLD_WIDTH; j++)
            {
                Color color;
                if (world[i][j] == 'X')
                {
                    color = BLACK;
                }
                else if (world[i][j] == '1')
                {
                    color = RED;
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

    return 0;
}
