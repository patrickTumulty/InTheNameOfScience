
#include "pray_engine.h"
#include "raylib.h"
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>

float sinwave(float frequency, float phase, float ampliture)
{
    return (sin(2 * PI * frequency * GetTime() + phase) * ampliture) * GetFrameTime();
}

Vector2 getRandomPosition(int xMin, int xMax, int yMin, int yMax)
{
    float x = (float) ((random() % xMax) + xMin);
    float y = (float) ((random() % yMax) + yMin);
    return (Vector2) {x, y};
}

int main(void)
{
    prayEngineInitialize();

    prayEngineRun();

    prayEngineDestroy();

    return 0;
}
