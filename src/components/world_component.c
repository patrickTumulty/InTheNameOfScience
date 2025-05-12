
#include "world_component.h"
#include "itnos_components.h"
#include "tmem.h"

constexpr static int WORLD_HEIGHT = 50;
constexpr static int WORLD_WIDTH = 50;
constexpr static int TILE_SIZE = 256;

void initWorldComponent(void *component)
{
    WorldComponent *worldComponent = (WorldComponent *) component;
    worldComponent->rows = WORLD_HEIGHT;
    worldComponent->cols = WORLD_WIDTH;
    worldComponent->tileSize = TILE_SIZE;

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

void deinitWorldComponent(void *component)
{
    WorldComponent *worldComponent = (WorldComponent *) component;
    tmemfree((void *) worldComponent->world);
    boolMatFree(worldComponent->navGrid);
}
