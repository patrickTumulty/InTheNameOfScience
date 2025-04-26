
#include "unit_system.h"
#include "system.h"


static void start()
{

}

static void stop()
{

}

void registerUnitSystem()
{
    System *system = systemNew();
    system->start = start;
    system->stop = stop;
}
