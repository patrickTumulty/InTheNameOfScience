
#include "pray_entity_registry.h"
#include "array_list.h"
#include "common_types.h"
#include "linked_list.h"
#include "pointer_list.h"
#include "pray_entity.h"
#include <raylib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

static PList entityList;

void entityRegistryInit()
{
    plistNew(&entityList, 0);
}

void entityRegistryDestroy()
{
    for (int i = 0; i < entityList.length; i++)
    {
        Entity *entity = plistGet(&entityList, i);
        if (entity == nullptr)
        {
            continue;
        }
        entityFree(entity);
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


Rc entityRegistryRegister(Entity *entity)
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

Rc entityRegistryUnregister(Entity *entity)
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

Entity *entityRegistryLookupFirst(const u32 *componentIDs, u32 componentIDsCount)
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
            void *comp = entityGetComponent(entity, componentID);
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

Rc entityRegistryLookupAll(LList *llist, const u32 *componentIDs, u32 componentIDsCount)
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
            void *comp = entityGetComponent(entity, componentID);
            if (comp != nullptr)
            {
                matches++;
            }
        }

        if (matches == componentIDsCount)
        {
            llistAppend(llist, &entity->lnode);
        }
    }
    return RC_OK;
}
