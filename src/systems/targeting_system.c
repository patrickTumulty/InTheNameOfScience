
#include "targeting_system.h"
#include "itnos_components.h"
#include "linked_list.h"
#include "pray_default_components.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_system.h"


static void gameUpdate()
{
    LList units;
    prayEntityLookupAll(&units, C(CID(TargetingComponent), CID(Transform2DComponent)), 2);

    LNode *node = nullptr;
    LListForEach(&units, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        Transform2DComponent *transform = getComponent(entity, Transform2DComponent);
        TargetingComponent *targeting = getComponent(entity, TargetingComponent);

        if (targeting->target == nullptr)
        {
            continue;
        }

        Transform2DComponent *targetTransform = getComponent(targeting->target, Transform2DComponent);
        UnitComponent *healthComponent = getComponent(targeting->target, UnitComponent);
    }
}

void registerTargetingSystem()
{
    System system = {
        .name = "Targeting",
        .gameUpdate = gameUpdate};
    praySystemsRegister(system);
}
