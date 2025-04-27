
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "common_types.h"
#include "linked_list.h"
#include <raylib.h>

typedef struct
{
    u32 id;
    u64 size;
    Consumer initialize;
    Consumer deinitialize;
    LNode lnode;
} ComponentInitializer;

Rc componentGetInitializer(u32 componentID, ComponentInitializer *componentInitialzer);
Rc componentRegister(u32 componentID,
                     u64 componentSize,
                     Consumer initCallback,
                     Consumer deinitCallback);
void componnentInitialize();
void componentsDestroy();

#endif // COMPONENTS_H
