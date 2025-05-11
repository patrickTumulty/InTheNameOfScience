
#include "camera_system.h"
#include "init_system.h"
#include "itnos_components.h"
#include "pray_engine.h"
#include "projectile_system.h"
#include "selection_system.h"
#include "sprite2d_system.h"
#include "path_system.h"
#include "turret_system.h"
#include "unit_system.h"
#include "world_system.h"
#include <stdlib.h>
#include <sys/types.h>

// float sinwave(float frequency, float phase, float ampliture)
// {
//     return (sin(2 * PI * frequency * GetTime() + phase) * ampliture) * GetFrameTime();
// }

Vector2 getRandomPosition(int xMin, int xMax, int yMin, int yMax)
{
    float x = (float) ((random() % xMax) + xMin);
    float y = (float) ((random() % yMax) + yMin);
    return (Vector2) {x, y};
}

int main(void)
{
    prayEngineInitialize();

    registerComponents();

    registerWorldSystem();
    registerInitSystem();
    registerPathSystem();
    registerTurretSystem();
    registerCameraSystem();
    registerSprite2DSystem();
    registerProjectileSystem();
    registerUnitSystem();
    registerSelectionSystem();

    prayEngineRun();

    prayEngineDestroy();

    return 0;
}
