
#include "sprite2d_system.h"
#include "itnos_components.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_default_components.h"
#include "pray_system.h"
#include "raylib.h"

static void renderUpdate()
{
    LList entities;
    Rc rc = prayEntityLookupAll(&entities, C(CID(Sprite2DComponent)), 1);
    if (rc != RC_OK)
    {
        return;
    }

    LNode *node = nullptr;
    LListForEach(&entities, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        Sprite2DComponent *sprite2d = getComponent(entity, Sprite2DComponent);

        Vector2 position = {0, 0};
        float rotationDegrees = 0;

        Transform2DComponent *transform = getComponent(entity, Transform2DComponent);
        if (transform != nullptr)
        {
            position = transform->position;
            rotationDegrees = transform->rotationDegrees;
        }

        Rectangle source = sprite2d->source;

        if (sprite2d->shader != nullptr)
        {
            BeginShaderMode(*sprite2d->shader);

            sprite2d->preShaderCallback(entity, sprite2d);
        }

        DrawTexturePro(sprite2d->texture,
                       sprite2d->source,
                       (Rectangle) {position.x, position.y, source.width, source.height},
                       sprite2d->origin,
                       rotationDegrees + sprite2d->rotationDegrees,
                       WHITE);

        if (sprite2d->shader != nullptr)
        {
            EndShaderMode();
        }
    }
}

void registerSprite2DSystem()
{
    System sprite2dSystem = {
        .name = "Sprite 2D System",
        .renderUpdateWorldSpace = renderUpdate,
    };
    praySystemsRegister(sprite2dSystem);
}
