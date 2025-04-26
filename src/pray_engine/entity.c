
#include "entity.h"
#include "array_list.h"
#include "common_types.h"
#include "components.h"
#include "linked_list.h"
#include "tmem.h"


static u64 entityCounter = 0;

Entity *entityNew(ComponentID *cids, u32 cidsLen)
{
    u64 size = sizeof(Entity);
    for (int i = 0; i < cidsLen; i++)
    {
        size += getComponentInitializer(cids[i]).size;
    }

    Entity *newEntity = tmemcalloc(1, size);
    if (newEntity == nullptr)
    {
        return nullptr;
    }

    newEntity->entityId = entityCounter++;
    alistNew(&newEntity->componentLookup, cidsLen, sizeof(ComponentPtr));
    llistInitNode(&newEntity->lnode, newEntity);

    u8 *ptr = (u8 *) newEntity;
    ptr += sizeof(Entity);

    int cidx = 0;
    for (int i = 0; i < cidsLen; i++)
    {
        ComponentID cid = cids[i];
        auto initializer = getComponentInitializer(cid);
        ComponentPtr *cptr = alistGet(&newEntity->componentLookup, cidx++);
        cptr->cid = cid;
        cptr->component = ptr;
        if (initializer.initialize != nullptr)
        {
            initializer.initialize(ptr);
        }
        ptr += initializer.size;
    }

    return newEntity;
}


Entity *entityFree(Entity *entity)
{
    for (int i = 0; i < entity->componentLookup.length; i++)
    {
        ComponentPtr *cptr = alistGet(&entity->componentLookup, i);
        ComponentInitializer initializer = getComponentInitializer(cptr->cid);
        if (initializer.deinitialize != nullptr)
        {
            initializer.deinitialize(cptr->component);
        }
    }
    tmemfree(entity);
    return nullptr;
}


void *entityGetComponent(Entity *entity, ComponentID cid)
{
    if (entity == nullptr)
    {
        return nullptr;
    }

    for (int i = 0; i < entity->componentLookup.length; i++)
    {
        ComponentPtr *cptr = alistGet(&entity->componentLookup, i);
        if (cptr->cid == cid)
        {
            return cptr->component;
        }
    }

    return nullptr;
}
