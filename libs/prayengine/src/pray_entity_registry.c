
#include "pray_entity_registry.h"
#include "common_types.h"
#include "linked_list.h"
#include "pointer_list.h"
#include "pray_entity.h"
#include <raylib.h>
#include <stdarg.h>
#include <stdint.h>

static PList entityList;

void prayEntityRegistryInit()
{
    plistNew(&entityList, 0);
}

void prayEntityRegistryDestroy()
{
    for (int i = 0; i < entityList.length; i++)
    {
        Entity *entity = plistGet(&entityList, i);
        if (entity == nullptr)
        {
            continue;
        }
        prayEntityFree(entity);
    }
    plistFree(&entityList);
}

static bool isEntityRegistered(Entity *entity)
{
    for (int i = 0; i < entityList.length; i++)
    {
        Entity *e = plistGet(&entityList, i);
        if (e == nullptr)
        {
            continue;
        }
        if (e == entity)
        {
            return true;
        }
    }
    return false;
}


Rc prayEntityRegister(Entity *entity)
{
    if (isEntityRegistered(entity))
    {
        return RC_BAD_PARAM;
    }
    for (int i = 0; i < entityList.length; i++)
    {
        Entity *e = plistGet(&entityList, i);
        if (e == nullptr)
        {
            return plistSet(&entityList, i, entity);
        }
    }
    return plistAppend(&entityList, entity);
}

Rc prayEntityUnregister(Entity *entity)
{
    for (int i = 0; i < entityList.length; i++)
    {
        Entity *e = plistGet(&entityList, i);
        if (e == entity)
        {
            return plistSet(&entityList, i, nullptr);
        }
    }
    return RC_NOT_FOUND;
}

Entity *prayEntityLookup(const u32 *componentIDs, u32 componentIDsCount)
{
    Entity *entity = nullptr;
    bool entityFound = false;
    for (int i = 0; i < entityList.length; i++)
    {
        entity = plistGet(&entityList, i);
        if (entity == nullptr)
        {
            continue;
        }
        int matches = 0;
        for (int j = 0; j < componentIDsCount; j++)
        {
            u32 componentID = componentIDs[j];
            void *comp = prayEntityGetComponent(entity, componentID);
            if (comp != nullptr)
            {
                matches++;
            }
        }
        if (matches == componentIDsCount)
        {
            entityFound = true;
            goto EXIT;
        }
    }

EXIT:

    return entityFound ? entity : nullptr;
}

Rc prayEntityLookupAll(LList *llist, const u32 *componentIDs, u32 componentIDsCount)
{
    llistInit(llist);
    for (int i = 0; i < entityList.length; i++)
    {
        Entity *entity = plistGet(&entityList, i);
        if (entity == nullptr)
        {
            continue;
        }

        int matches = 0;
        for (int j = 0; j < componentIDsCount; j++)
        {
            u32 componentID = componentIDs[j];
            void *comp = prayEntityGetComponent(entity, componentID);
            if (comp != nullptr)
            {
                matches++;
            }
        }

        if (matches == componentIDsCount)
        {
            entity->lnode.next = nullptr;
            entity->lnode.prev = nullptr;
            llistAppend(llist, &entity->lnode);
        }
    }
    return RC_OK;
}
