
#ifndef ENTITY_H
#define ENTITY_H


#include "array_list.h"
#include "components.h"
#include "linked_list.h"

typedef struct {
    ComponentID cid;
    void *component;
} ComponentPtr;

typedef struct {
    u32 entityId;
    AList componentLookup;
    LNode lnode;
} Entity;

Entity *entityNew(ComponentID *cids, u32 cidsLen);
Entity *entityFree(Entity *entity);
void *entityGetComponent(Entity *entity, ComponentID cid);


#endif // ENTITY_H
