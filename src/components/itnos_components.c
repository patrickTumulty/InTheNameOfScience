
#include "itnos_components.h"

#include "pathfind_component.h"
#include "pray_component.h"
#include "world_component.h"
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


char *componentID2Str(ComponentID id)
{
    switch (id)
    {
        case CID_RESERVED: return "RESERVED";
        case CID_WORLD: return "WORLD";
        case CID_TRANSFORM: return "TRANSFORM";
        case CID_UNIT: return "UNIT";
        case CID_PATHFINDING: return "PATHFINDING";
        case CID_SPRITE_2D: return "SPRITE_2D";
        case CID_COLLIDER_2D: return "COLLIDER_2D";
        case CID_SELECTABLE: return "SELECTABLE";
        case CID_HEALTH: return "HEALTH";
        case CID_DAMAGE: return "DAMAGE";
        case CID_ENEMY: return "ENEMY";
        default: return "UNKONWN";
    }
}
