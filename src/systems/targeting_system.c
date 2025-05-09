
#include "targeting_system.h"
#include "itnos_components.h"
#include "linked_list.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_system.h"


static void gameUpdate()
{

    LList units;
    prayEntityLookupAll(&units, C(CID_TARGETING, CID_TRANSFORM), 2);

    LNode *node = nullptr;
    LListForEach(&units, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        TransformComponent *transform = prayEntityGetComponent(entity, CID_TRANSFORM);
        TargetingComponent *targeting = prayEntityGetComponent(entity, CID_TARGETING);

        if (targeting->target == nullptr)
        {
            continue;
        }

        TransformComponent *targetTransform = prayEntityGetComponent(targeting->target, CID_TRANSFORM);
        HealthComponent *healthComponent = prayEntityGetComponent(targeting->target, CID_HEALTH);


    }
}

void registerTargetingSystem()
{
    System system = {
        .name = "Targeting",
        .gameUpdate = gameUpdate};
    praySystemsRegister(system);
}
