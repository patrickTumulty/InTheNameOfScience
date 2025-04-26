
#include "entity_registry.h"
#include "array_list.h"
#include "common_types.h"
#include "entity.h"
#include "linked_list.h"
#include <raylib.h>
#include <stdarg.h>
#include <stdint.h>

static AList entityList;


void entityRegistryInit()
{
    alistNew(&entityList, 10, sizeof(AListPtr));
    for (int i = 0; i < entityList.length; i++)
    {
        alistSet(&entityList, i, nullptr);
    }
}

void entityRegistryDestroy()
{
    for (int i = 0; i < entityList.length; i++)
    {
        AListPtr *ptr = alistGet(&entityList, i);
        Entity *entity = ptr->ptr;
        if (entity == nullptr)
        {
            continue;
        }
        entityFree(entity);
    }
    alistFree(&entityList);
}

Rc entityRegistryRegister(Entity *entity)
{
    for (int i = 0; i < entityList.length; i++)
    {
        AListPtr *ptr = alistGet(&entityList, i);
        if (ptr->ptr == nullptr)
        {
            return alistSet(&entityList, 0, alistptr(entity));
        }
    }
    return alistAppend(&entityList, alistptr(entity));
}

Rc entityRegistryUnregister(Entity *entity)
{
    for (int i = 0; i < entityList.length; i++)
    {
        AListPtr *ptr = alistGet(&entityList, i);
        if (ptr->ptr == entity)
        {
            return alistSet(&entityList, i, nullptr);
        }
    }
    return RC_NOT_FOUND;
}

Entity *entityRegistryLookupFirst(ComponentID *cids, u32 cidsLen)
{
    for (int i = 0; i < entityList.length; i++)
    {
        AListPtr *ptr = alistGet(&entityList, i);
        Entity *entity = ptr->ptr;
        if (entity == nullptr)
        {
            continue;
        }
        int matches = 0;
        for (int j = 0; j < cidsLen; j++)
        {
            void *comp = entityGetComponent(entity, cids[j]);
            if (comp != nullptr)
            {
                matches++;
            }
        }
        if (matches == cidsLen)
        {
            return entity;
        }
    }
    return nullptr;
}

Rc entityRegistryLookupAll(LList *llist, ComponentID *cids, u32 cidsLen)
{
    llistInit(llist);
    for (int i = 0; i < entityList.length; i++)
    {
        Entity *entity = alistGet(&entityList, i);
        if (entity == nullptr)
        {
            continue;
        }
        int matches = 0;
        for (int j = 0; j < cidsLen; j++)
        {
            void *comp = entityGetComponent(entity, cids[j]);
            if (comp != nullptr)
            {
                matches++;
            }
        }
        if (matches == cidsLen)
        {
            llistAppend(llist, &entity->lnode);
        }
    }
    return RC_OK;
}
