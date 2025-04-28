
#include "itnos_components.h"

#include "bool_mat.h"
#include "pray_component.h"
#include "pray_globals.h"
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


void registerComponents()
{
    componentRegister(CID_WORLD, sizeof(WorldComponent), initWorldComponent, deinitWorldComponent);
}
