
#include "object_registry.h"
#include "array_list.h"
#include <raylib.h>
#include <stdarg.h>
#include <stdint.h>

static AList entityList;
static u64 entityIDCounter = 0;



void initObjectRegistry()
{
    alistNew(&entityList, 10, sizeof(void *));
    for (int i = 0; i < entityList.length; i++)
    {
        alistSet(&entityList, 0, nullptr);
    }
}

void destroyObjectRegistry()
{
}


Rc registerComponent(ComponentID id, u64 componentSize)
{
}
