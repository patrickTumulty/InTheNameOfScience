
#include "pray_system.h"
#include "array_list.h"

static AList systems;


void systemNoop(void)
{
    // no-op
}

static void clearSystem(System *system)
{
    system->stop = systemNoop;
    system->start = systemNoop;
    system->gameUpdate = systemNoop;
    system->renderUpdateWorldSpace = systemNoop;
    system->renderUpdateScreenSpace = systemNoop;
    system->initialized = false;
    system->started = false;
    system->systemName[0] = '\0';
}

static System *getUninitializedSystemPointer()
{
    for (int i = 0; i < systems.length; i++)
    {
        System *system = alistGet(&systems, i);
        if (!system->initialized)
        {
            clearSystem(system);
            system->initialized = true;
            return system;
        }
    }
    u32 len = systems.length;
    alistResize(&systems, len + 1);
    System *system = alistGet(&systems, (int) len);
    clearSystem(system);
    system->initialized = true;
    return system;
}

System *systemNew()
{
    return getUninitializedSystemPointer();
}

void systemsInit()
{
    alistNew(&systems, 0, sizeof(System));
}

void systemsDestroy()
{
    alistFree(&systems);
}

AList *systemsGet()
{
    return &systems;
}

Rc systemsAdd(System *system)
{
    // TODO
}

Rc systemsRemove(System *system)
{
    // TODO
}

void systemsRunStart()
{
    for (int i = 0; i < systems.length; i++)
    {
        System *system = alistGet(&systems, i);
        system->start();
    }
}

void systemsRunStop()
{
    for (int i = 0; i < systems.length; i++)
    {
        System *system = alistGet(&systems, i);
        system->stop();
    }
}

void systemsRunGameUpdate()
{
    for (int i = 0; i < systems.length; i++)
    {
        System *system = alistGet(&systems, i);
        system->gameUpdate();
    }
}

void systemsRunRenderUpdateWorldSpace()
{
    for (int i = 0; i < systems.length; i++)
    {
        System *system = alistGet(&systems, i);
        system->renderUpdateWorldSpace();
    }
}

void systemsRunRenderUpdateScreenSpace()
{
    for (int i = 0; i < systems.length; i++)
    {
        System *system = alistGet(&systems, i);
        system->renderUpdateScreenSpace();
    }
}
