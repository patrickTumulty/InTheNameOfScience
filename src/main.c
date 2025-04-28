
#include "bool_mat.h"
#include "camera_system.h"
#include "itnos_components.h"
#include "pathfinding_system.h"
#include "pray_engine.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "raylib.h"
#include "world_system.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// float sinwave(float frequency, float phase, float ampliture)
// {
//     return (sin(2 * PI * frequency * GetTime() + phase) * ampliture) * GetFrameTime();
// }
//
// Vector2 getRandomPosition(int xMin, int xMax, int yMin, int yMax)
// {
//     float x = (float) ((random() % xMax) + xMin);
//     float y = (float) ((random() % yMax) + yMin);
//     return (Vector2) {x, y};
// }

int main(void)
{
    prayEngineInitialize();

    registerComponents();

    registerWorldSystem();
    registerPathingSystem();
    registerCameraSystem();

    prayEngineRun();

    prayEngineDestroy();

    return 0;
}
