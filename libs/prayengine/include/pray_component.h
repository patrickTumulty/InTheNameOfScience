
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "common_types.h"
#include "linked_list.h"
#include <raylib.h>

#define COMPONENT_BANK_DEFAULT 1
#define COMPONENT_BANK_A 100
#define COMPONENT_BANK_B 200
#define COMPONENT_BANK_C 300
#define COMPONENT_BANK_D 400

typedef u32 cid; // component id

#define REGISTER_CID(COMPONENT)                          \
    enum : cid                                           \
    {                                                    \
        CID_##COMPONENT = COMPONENT_OFFSET + __COUNTER__ \
    };

#define CID(COMPONENT) (CID_##COMPONENT)

#define C(...) \
    (cid[]) { __VA_ARGS__ }

typedef struct
{
    u32 id;
    u64 size;
    Consumer initialize;
    Consumer deinitialize;
    LNode lnode;
} ComponentInitializer;

Rc prayComponentGetInitializer(cid componentID, ComponentInitializer *componentInitialzer);
Rc prayComponentRegister(cid componentID,
                         u64 componentSize,
                         Consumer initCallback,
                         Consumer deinitCallback);
void prayComponnentInitialize();
void prayComponentsDestroy();

#define registerComponent(TYPE, INIT, DEINIT) prayComponentRegister(CID(TYPE), sizeof(TYPE), INIT, DEINIT)

#endif // COMPONENTS_H
