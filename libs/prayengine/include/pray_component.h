
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

Rc prayComponentGetInitializer(u32 componentID, ComponentInitializer *componentInitialzer);
Rc prayComponentRegister(u32 componentID,
                     u64 componentSize,
                     Consumer initCallback,
                     Consumer deinitCallback);
void prayComponnentInitialize();
void prayComponentsDestroy();

#endif // COMPONENTS_H
