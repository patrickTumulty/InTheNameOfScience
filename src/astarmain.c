
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

    SetTargetFPS(60);

    Position startingPoint = {1, 1};

    while (!WindowShouldClose())
    {

        BeginDrawing();

        ClearBackground(RAYWHITE);


        EndDrawing();
    }

    CloseWindow();

    tMemDestroy();

    return 0;
}
