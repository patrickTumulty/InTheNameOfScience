
#ifndef OBJECT_REGISTRY_H
#define OBJECT_REGISTRY_H

#include "common_types.h"
#include "pray_entity.h"
#include "linked_list.h"
#include <raylib.h>
#include <stdint.h>

void entityRegistryInit();
void entityRegistryDestroy();
Rc entityRegistryRegister(Entity *entity);
Rc entityRegistryUnregister(Entity *entity);
Entity *entityRegistryLookupFirst(u32 componentsCount, ...);
Rc entityRegistryLookupAll(LList *list, u32 componentsCount, ...);

#endif
