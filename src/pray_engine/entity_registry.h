
#ifndef OBJECT_REGISTRY_H
#define OBJECT_REGISTRY_H

#include "common_types.h"
#include "entity.h"
#include "linked_list.h"
#include <raylib.h>
#include <stdint.h>
#include "components.h"

void entityRegistryInit();
void entityRegistryDestroy();
Rc entityRegistryRegister(Entity *entity);
Rc entityRegistryUnregister(Entity *entity);
Entity *entityRegistryLookupFirst(ComponentID *cids, u32 cidsLen);
Rc entityRegistryLookupAll(LList *list, ComponentID *cids, u32 cidsLen);

#endif
