
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

Entity *entityNew(const u32 *componentIDs,u32 componentIDsCount) ;
Entity *entityFree(Entity *entity);
void *entityGetComponent(Entity *entity, u32 componentID);


#endif // ENTITY_H
