
#include "itnos_components.h"

#include "bool_mat.h"
#include "pray_component.h"
#include "pray_globals.h"
#include "raylib.h"
#include "tmem.h"
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <threads.h>


static void initWorldComponent(void *component)
{
    WorldComponent *worldComponent = (WorldComponent *) component;
    worldComponent->rows = WORLD_HEIGHT;
    worldComponent->cols = WORLD_WIDTH;

    u64 rows = worldComponent->rows;
    u64 cols = worldComponent->cols;

    u64 size = (rows * sizeof(char *)) + (rows * cols * sizeof(char));
    worldComponent->world = (char **) tmemcalloc(1, size);

    u8 *data = (u8 *) (worldComponent->world + rows);

    for (u64 i = 0; i < worldComponent->rows; i++)
    {
        worldComponent->world[i] = (char *) (data + (worldComponent->cols * i));
    }

    worldComponent->navGrid = boolMatNew(worldComponent->rows, worldComponent->cols, true, false);
}

static void deinitWorldComponent(void *component)
{
    WorldComponent *worldComponent = (WorldComponent *) component;
    tmemfree((void *) worldComponent->world);
    boolMatFree(worldComponent->navGrid);
}

static void initPathComponent(void *component)
{
    PathfindComponent *pathfindComponent = (PathfindComponent *) component;

    pathfindComponent->path.path = nullptr;
    pathfindComponent->path.pathLen = 0;
    pathfindComponent->pathSet = false;
    pathfindComponent->index = 0;
    pathfindComponent->speed = 200;
}

static void freePathComponent(void *component)
{
    PathfindComponent *pathfindComponent = (PathfindComponent *) component;
    if (pathfindComponent->path.path != nullptr)
    {
        tmemfree(pathfindComponent->path.path);
        pathfindComponent->path.path = nullptr;
    }
}

void registerComponents()
{
    componentRegister(CID_WORLD, sizeof(WorldComponent), initWorldComponent, deinitWorldComponent);
    componentRegister(CID_TRANSFORM, sizeof(TransformComponent), nullptr, nullptr);
    componentRegister(CID_UNIT, 0, nullptr, nullptr);
    componentRegister(CID_PATHFINDING, sizeof(PathfindComponent), initPathComponent, freePathComponent);
}
