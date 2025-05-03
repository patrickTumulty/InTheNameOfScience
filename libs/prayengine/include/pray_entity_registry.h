
#ifndef OBJECT_REGISTRY_H
#define OBJECT_REGISTRY_H

#include "common_types.h"
#include "pray_entity.h"
#include "linked_list.h"
#include <raylib.h>
#include <stdint.h>

void prayEntityRegistryInit();
void prayEntityRegistryDestroy();
Rc prayEntityRegister(Entity *entity);
Rc prayEntityUnregister(Entity *entity);
Entity *prayEntityLookup(const u32 *componentIDs, u32 componentIDsCount);
Rc prayEntityLookupAll(LList *llist, const u32 *componentIDs, u32 componentIDsCount);

#endif
