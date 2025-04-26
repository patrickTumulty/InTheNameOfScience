
#include "system.h"
#include "array_list.h"
#include "tmem.h"
#include <stdlib.h>

static AList systemsList;


void systemNoop(void)
{
    // no-op
}

static void clearSystem(System *system)
{
    system->close = systemNoop;
    system->start = systemNoop;
    system->gameUpdate = systemNoop;
    system->renderUpdate = systemNoop;
    system->initialized = false;
    system->started = false;
    system->systemName[0] = '\0';
}

static System *getUninitializedSystemPointer()
{
    for (int i = 0; i < systemsList.length; i++)
    {
        System *system = alistGet(&systemsList, i);
        if (!system->initialized) 
        {
            return system;
        }
    }
    u32 len = systemsList.length;
    alistResize(&systemsList, len + 1);
    return alistGet(&systemsList, (int) len);
}

System *systemNew()
{
    System *newSystem = getUninitializedSystemPointer();
    if (newSystem == nullptr)
    {
        return nullptr;
    }

    clearSystem(newSystem);

    return newSystem;
}

void systemsInit()
{
    alistNew(&systemsList, 0, sizeof(System));
}

void systemsDestroy()
{
    alistFree(&systemsList);
}

AList *systemsGet()
{
    return &systemsList;
}

Rc systemsAdd(System *system)
{
    // TODO
}

Rc systemsRemove(System *system)
{
    // TODO
}
