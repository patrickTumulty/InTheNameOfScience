
#include "itnos_components.h"
#include "pathfind_component.h"
#include "pray_component.h"
#include "pray_default_components.h"
#include "world_component.h"
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <threads.h>


void registerComponents()
{
    prayRegisterDefaultComponents();

    registerComponent(WorldComponent, initWorldComponent, deinitWorldComponent);
    registerComponent(UnitComponent, nullptr, nullptr);
    registerComponent(PathfindComponent, initPathfindComponent, freePathfindComponent);
    registerComponent(Collider2DComponent, nullptr, nullptr);
    registerComponent(SelectableComponent, nullptr, nullptr);
    registerComponent(HealthComponent, nullptr, nullptr);
    registerComponent(TargetingComponent, nullptr, nullptr);
    registerComponent(EnemyComponent, nullptr, nullptr);
    registerComponent(ProjectileComponent, nullptr, nullptr);
    registerComponent(TargetingComponent, nullptr, nullptr);
    registerComponent(TurretComponent, nullptr, nullptr);
    registerComponent(TargetComponent, nullptr, nullptr);
}

