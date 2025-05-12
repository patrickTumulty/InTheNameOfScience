
#include "pray_component.h"
#include "common_types.h"
#include "linked_list.h"
#include "tmem.h"
#include <raylib.h>

cid globalCidCounter;
static LList componentsList;

static ComponentInitializer *findComponentInitializer(cid componentID)
{
    LNode *lnode = nullptr;
    LListForEach(&componentsList, lnode)
    {
        ComponentInitializer *compInitializer = LListGetEntry(lnode, ComponentInitializer);
        if (compInitializer == nullptr)
        {
            continue;
        }
        if (compInitializer->id == componentID)
        {
            return compInitializer;
        }
    }
    return nullptr;
}

Rc prayComponentGetInitializer(cid componentID, ComponentInitializer *componentInitialzer)
{
    ComponentInitializer *compInitializer = findComponentInitializer(componentID);
    if (compInitializer == nullptr)
    {
        return RC_NOT_FOUND;
    }
    *componentInitialzer = *compInitializer;
    return RC_OK;
}

Rc prayComponentRegister(cid componentID,
                         u64 componentSize,
                         Consumer initCallback,
                         Consumer deinitCallback)
{
    ComponentInitializer *compInitializer = findComponentInitializer(componentID);
    if (compInitializer != nullptr)
    {
        return RC_BAD_PARAM;
    }

    compInitializer = tmemcalloc(1, sizeof(ComponentInitializer));
    compInitializer->id = componentID;
    compInitializer->size = componentSize;
    compInitializer->initialize = initCallback;
    compInitializer->deinitialize = deinitCallback;
    llistInitNode(&compInitializer->lnode, compInitializer);
    llistAppend(&componentsList, &compInitializer->lnode);

    return RC_OK;
}

void prayComponnentInitialize()
{
    llistInit(&componentsList);
}

void prayComponentsDestroy()
{
    LNode *node = llistPopFront(&componentsList);
    while (node != nullptr)
    {
        ComponentInitializer *compInitializer = LListGetEntry(node, ComponentInitializer);
        tmemfree(compInitializer);
        node = llistPopFront(&componentsList);
    }
}

cid nextCid()
{

}
