
#include "pray_entity_registry.h"
#include "array_list.h"
#include "common_types.h"
#include "linked_list.h"
#include "pray_entity.h"
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

Entity *entityRegistryLookupFirst(u32 componentsCount, ...)
{
    va_list args;
    va_start(args, componentsCount);

    Entity *entity = nullptr;
    for (int i = 0; i < entityList.length; i++)
    {
        AListPtr *ptr = alistGet(&entityList, i);
        entity = ptr->ptr;
        if (entity == nullptr)
        {
            continue;
        }
        int matches = 0;
        for (int j = 0; j < componentsCount; j++)
        {
            u32 componentID = va_arg(args, u32);
            void *comp = entityGetComponent(entity, componentID);
            if (comp != nullptr)
            {
                matches++;
            }
        }
        if (matches == componentsCount)
        {
            goto EXIT;
        }
    }

EXIT:
    va_end(args);
    return entity;
}

Rc entityRegistryLookupAll(LList *llist, u32 componentsCount, ...)
{
    va_list args;
    va_start(args, componentsCount);

    llistInit(llist);
    for (int i = 0; i < entityList.length; i++)
    {
        Entity *entity = alistGet(&entityList, i);
        if (entity == nullptr)
        {
            continue;
        }

        int matches = 0;
        for (int j = 0; j < componentsCount; j++)
        {
            u32 componentID = va_arg(args, u32);
            void *comp = entityGetComponent(entity, componentID);
            if (comp != nullptr)
            {
                matches++;
            }
        }

        if (matches == componentsCount)
        {
            llistAppend(llist, &entity->lnode);
        }
    }

    va_end(args);

    return RC_OK;
}
