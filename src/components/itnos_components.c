
#include "itnos_components.h"

#include "bool_mat.h"
#include "common_utils.h"
#include "linked_list.h"
#include "pathfind_component.h"
#include "pray_component.h"
#include "pray_globals.h"
#include "raylib.h"
#include "tmem.h"
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <threads.h>

static void initHealth(void *component)
{
    HealthComponent *healthComponent = component;
    healthComponent->maxHealth = 100;
    healthComponent->currentHealth = 100;
}



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
    prayComponentRegister(CID_WORLD, sizeof(WorldComponent), initWorldComponent, deinitWorldComponent);
    prayComponentRegister(CID_TRANSFORM, sizeof(TransformComponent), nullptr, nullptr);
    prayComponentRegister(CID_UNIT, 0, nullptr, nullptr);
    prayComponentRegister(CID_PATHFINDING, sizeof(PathfindComponent), initPathfindComponent, freePathfindComponent);
    prayComponentRegister(CID_SPRITE_2D, sizeof(Sprite2DComponent), nullptr, nullptr);
    prayComponentRegister(CID_COLLIDER_2D, sizeof(Collider2DComponent), nullptr, nullptr);
    prayComponentRegister(CID_SELECTABLE, sizeof(SelectableComponent), nullptr, nullptr);
    prayComponentRegister(CID_HEALTH, sizeof(HealthComponent), initHealth, nullptr);
    prayComponentRegister(CID_DAMAGE, sizeof(DamageComponent), nullptr, nullptr);
    prayComponentRegister(CID_ENEMY, 0, nullptr, nullptr);
}
