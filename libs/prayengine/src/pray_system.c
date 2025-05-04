
#include "pray_system.h"
#include "array_list.h"
#include <pthread.h>
#include <stdio.h>

static AList systems;


void praySystemNoop(void)
{
    // no-op
}

void praySystemsInit()
{
    alistNew(&systems, 0, sizeof(System));
}

void praySystemsDestroy()
{
    alistFree(&systems);
}

AList *praySystemsGetList()
{
    return &systems;
}

#define SET_NOOP_IF_NULL(A) (A) = (A) != nullptr ? (A) : praySystemNoop

Rc praySystemsRegister(System system)
{
    SET_NOOP_IF_NULL(system.start);
    SET_NOOP_IF_NULL(system.stop);
    SET_NOOP_IF_NULL(system.gameUpdate);
    SET_NOOP_IF_NULL(system.renderUpdateScreenSpace);
    SET_NOOP_IF_NULL(system.renderUpdateWorldSpace);
    return alistAppend(&systems, &system);
}

void praySystemsRunStart()
{
    for (int i = 0; i < systems.length; i++)
    {
        System *system = alistGet(&systems, i);
        system->start();
    }
}

void praySystemsRunStop()
{
    for (int i = 0; i < systems.length; i++)
    {
        System *system = alistGet(&systems, i);
        system->stop();
    }
}

void praySystemsRunGameUpdate()
{
    for (int i = 0; i < systems.length; i++)
    {
        System *system = alistGet(&systems, i);
        system->gameUpdate();
    }
}

void praySystemsRunRenderUpdateWorldSpace()
{
    for (int i = 0; i < systems.length; i++)
    {
        System *system = alistGet(&systems, i);
        system->renderUpdateWorldSpace();
    }
}

void praySystemsRunRenderUpdateScreenSpace()
{
    for (int i = 0; i < systems.length; i++)
    {
        System *system = alistGet(&systems, i);
        system->renderUpdateScreenSpace();
    }
}
