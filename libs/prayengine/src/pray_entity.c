
#include "pray_entity.h"
#include "array_list.h"
#include "common_types.h"
#include "pray_component.h"
#include "linked_list.h"
#include <stdarg.h>
#include "tmem.h"

static u64 entityCounter = 0;

Entity *entityNew(u32 componentsCount, ...)
{
    va_list args;
    va_list args_copy;
    va_start(args, componentsCount); // Initialize args to store all values after count

    u64 size = sizeof(Entity);
    for (int i = 0; i < componentsCount; i++)
    {
        u32 componentID = va_arg(args, u32);
        ComponentInitializer compInitializer = {0};
        if (componentGetInitializer(componentID, &compInitializer) != RC_OK) {
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
    alistNew(&newEntity->componentLookup, componentsCount, sizeof(ComponentPtr));
    llistInitNode(&newEntity->lnode, newEntity);

    u8 *ptr = (u8 *) newEntity;
    ptr += sizeof(Entity);

    va_start(args, componentsCount);
    va_copy(args_copy, args);

    int cidx = 0;
    for (int i = 0; i < componentsCount; i++)
    {
        u32 componentID = va_arg(args_copy, u32);
        ComponentInitializer compInitializer = {0};
        if (componentGetInitializer(componentID, &compInitializer) != RC_OK) {
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

    va_end(args);
    va_end(args_copy);

    return newEntity;
}

Entity *entityFree(Entity *entity)
{
    for (int i = 0; i < entity->componentLookup.length; i++)
    {
        ComponentPtr *cptr = alistGet(&entity->componentLookup, i);
        ComponentInitializer compInitializer = {0};
        if (componentGetInitializer(cptr->componentID, &compInitializer) != RC_OK) {
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

void *entityGetComponent(Entity *entity, u32 componentID)
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
