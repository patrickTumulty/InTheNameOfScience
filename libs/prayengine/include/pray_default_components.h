
#ifndef PRAY_COMPONENTS_H
#define PRAY_COMPONENTS_H

#include "pray_entity.h"
#include "raylib.h"
#include "pray_component.h"

#ifdef COMPONENT_OFFSET
#undef COMPONENT_OFFSET
#endif
#define COMPONENT_OFFSET COMPONENT_BANK_DEFAULT

typedef struct 
{
    Vector2 position;
    float rotationDegrees; // 0.0 to 359.0
} Transform2DComponent;

REGISTER_CID(Transform2DComponent);

constexpr char str[] = "Hello";

struct Sprite2DComponent;

typedef void (*PreShaderCallback)(Entity *entity, struct Sprite2DComponent *sprite2D);

typedef struct Sprite2DComponent
{
    Texture2D texture;
    Rectangle source;
    Vector2 origin;
    float rotationDegrees; // degrees
    Shader *shader;
    PreShaderCallback preShaderCallback;
} Sprite2DComponent;

REGISTER_CID(Sprite2DComponent);

void prayRegisterDefaultComponents();

#endif // PRAY_COMPONENTS_H
