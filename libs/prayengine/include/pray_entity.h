
#ifndef ENTITY_H
#define ENTITY_H

#include "array_list.h"
#include "linked_list.h"

typedef struct {
    u32 componentID;
    void *componentPtr;
} ComponentPtr;

typedef struct {
    u32 entityId;
    AList componentLookup;
    LNode lnode;
} Entity;

Entity *prayEntityNew(const u32 *componentIDs,u32 componentIDsCount) ;
Entity *prayEntityFree(Entity *entity);
void *prayEntityGetComponent(Entity *entity, u32 componentID);

#endif // ENTITY_H
