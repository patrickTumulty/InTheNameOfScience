
#ifndef ENTITY_H
#define ENTITY_H

#include "array_list.h"
#include "linked_list.h"

typedef struct
{
    u32 componentID;
    void **ptr;
} ComponentIDRef;

#define CID_REF_MAP_LEN(REFMAP) sizeof(REFMAP) / sizeof(ComponentIDRef);
#define CID_REF(CID, PTR) (ComponentIDRef){ (CID), (void**) (PTR)}

typedef struct
{
    u32 entityId;
    AList componentLookup;
    LNode lnode;
} Entity;

Entity *prayEntityNew(const u32 *componentIDs, u32 componentIDsCount);
Entity *prayEntityFree(Entity *entity);
void *prayEntityGetComponent(Entity *entity, u32 componentID);
void prayEntityGetComponents(Entity *entity, ComponentIDRef *refMap, u32 refMapLen);

#define getComponent(ENTITY, TYPE) (TYPE*) prayEntityGetComponent((ENTITY), CID(TYPE))

#endif // ENTITY_H
