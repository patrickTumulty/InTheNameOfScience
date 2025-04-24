
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "common_types.h"
#include <raylib.h>

typedef enum ComponentID : u16
{
    CID_EMPTY = 0, // Reserved for representing unintialized memory
    CID_PLAYER = 1,
    CID_TRANSFORM = 2,
} ComponentID;

typedef struct {
    ComponentID id;
    u64 size;
    void (*initialize)(void *);
    void (*deinitialize)(void *);
} ComponentInitializer;

typedef struct {
    Vector2 location;
} Transform2D;

ComponentInitializer getComponentInitializer(ComponentID cid);

#endif // COMPONENTS_H
