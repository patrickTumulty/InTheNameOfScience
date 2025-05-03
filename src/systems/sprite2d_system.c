
#include "sprite2d_system.h"
#include "itnos_components.h"
#include "pray_entity_registry.h"
#include "pray_system.h"
#include "raylib.h"

static void renderUpdate()
{
    LList entities;
    Rc rc = entityRegistryLookupAll(&entities, C(CID_SPRITE_2D), 1);
    if (rc != RC_OK)
    {
        return;
    }

    LNode *node = nullptr;
    LListForEach(&entities, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        Sprite2DComponent *sprite2d = entityGetComponent(entity, CID_SPRITE_2D);

        Vector2 position = {0, 0};
        float rotation = 0;

        TransformComponent *transform = entityGetComponent(entity, CID_TRANSFORM);
        if (transform != nullptr)
        {
            position = transform->position;
            rotation = transform->rotation;
        }

        Rectangle source = sprite2d->source;

        DrawTexturePro(sprite2d->texture,
                       sprite2d->source,
                       (Rectangle) {position.x, position.y, source.width, source.height},
                       sprite2d->origin,
                       rotation + sprite2d->rotation,
                       WHITE);
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
