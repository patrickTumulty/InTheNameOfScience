
#include "pray_entity.h"
#include "array_list.h"
#include "common_types.h"
#include "linked_list.h"
#include "pray_component.h"
#include "tmem.h"
#include <stdarg.h>

typedef struct
{
    u32 componentID;
    void *componentPtr;
} ComponentPtr;

static u64 entityCounter = 0;

Entity *prayEntityNew(const u32 *componentIDs, u32 componentIDsCount)
{
    u64 size = sizeof(Entity);
    for (int i = 0; i < componentIDsCount; i++)
    {
        u32 componentID = componentIDs[i];
        ComponentInitializer compInitializer = {0};
        if (prayComponentGetInitializer(componentID, &compInitializer) != RC_OK)
        {
            continue;
        }
        size += compInitializer.size;
    }

    Entity *newEntity = tmemcalloc(1, size);
    if (newEntity == nullptr)
    {
        return nullptr;
    }

    newEntity->entityId = entityCounter++;
    alistNew(&newEntity->componentLookup, componentIDsCount, sizeof(ComponentPtr));
    llistInitNode(&newEntity->lnode, newEntity);

    u8 *ptr = (u8 *) newEntity;
    ptr += sizeof(Entity);

    int cidx = 0;
    for (int i = 0; i < componentIDsCount; i++)
    {
        u32 componentID = componentIDs[i];
        ComponentInitializer compInitializer = {0};
        if (prayComponentGetInitializer(componentID, &compInitializer) != RC_OK)
        {
            continue;
        }
        ComponentPtr *cptr = alistGet(&newEntity->componentLookup, cidx++);
        cptr->componentID = componentID;
        cptr->componentPtr = ptr;
        if (compInitializer.initialize != nullptr)
        {
            compInitializer.initialize(ptr);
        }
        ptr += compInitializer.size;
    }

    return newEntity;
}

Entity *prayEntityFree(Entity *entity)
{
    for (int i = 0; i < entity->componentLookup.length; i++)
    {
        ComponentPtr *cptr = alistGet(&entity->componentLookup, i);
        ComponentInitializer compInitializer = {0};
        if (prayComponentGetInitializer(cptr->componentID, &compInitializer) != RC_OK)
        {
            continue;
        }
        if (compInitializer.deinitialize != nullptr)
        {
            compInitializer.deinitialize(cptr->componentPtr);
        }
    }
    alistFree(&entity->componentLookup);
    tmemfree(entity);
    return nullptr;
}

void *prayEntityGetComponent(Entity *entity, u32 componentID)
{
    if (entity == nullptr)
    {
        return nullptr;
    }

    for (int i = 0; i < entity->componentLookup.length; i++)
    {
        ComponentPtr *cptr = alistGet(&entity->componentLookup, i);
        if (cptr->componentID == componentID)
        {
            return cptr->componentPtr;
        }
    }

    return nullptr;
}


void prayEntityGetComponents(Entity *entity, ComponentIDRef *refMap, u32 refMapLen)
{
    if (entity == nullptr)
    {
        return;
    }

    for (int i = 0; i < entity->componentLookup.length; i++)
    {
        ComponentPtr *cptr = alistGet(&entity->componentLookup, i);
        for (int j = 0; j < refMapLen; j++)
        {
            if (cptr->componentID == refMap[j].componentID)
            {
                (*refMap[j].ptr) = cptr->componentPtr;
            }
        }
    }
}
