
#include "selection_system.h"
#include "common_utils.h"
#include "itnos_components.h"
#include "linked_list.h"
#include "pointer_list.h"
#include "pray_camera.h"
#include "pray_default_components.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "pray_system.h"
#include "pray_utils.h"
#include "raylib.h"
#include <math.h>
#include <string.h>

static int selectedEntitiesCount = 0;
static PList selectedEntities;
static Color selectionColor = GREEN;
static Color selectionBorderColor = GREEN;
static Vector2 mouseDownPoint;
static Vector2 mouseDragPoint;
static Rectangle selectionRectangle;

static void start()
{
    selectionColor.a = 70;
    plistNew(&selectedEntities, 10);
}

static bool entitySelected(Entity *entity)
{
    // TODO: Use a SET here instead of linear search
    for (int i = 0; i < selectedEntitiesCount; i++)
    {
        Entity *selected = plistGet(&selectedEntities, i);
        if (selected->entityId == entity->entityId)
        {
            return true;
        }
    }
    return false;
}

static void addSelectedEntity(Entity *entity)
{
    if (entitySelected(entity))
    {
        return;
    }

    if (selectedEntitiesCount < selectedEntities.length)
    {
        plistSet(&selectedEntities, selectedEntitiesCount, entity);
    }
    else
    {
        plistAppend(&selectedEntities, entity);
    }
    selectedEntitiesCount++;
}

static void clearSelectedEntities()
{
    for (int i = 0; i < selectedEntitiesCount; i++)
    {
        Entity *entity = plistGet(&selectedEntities, i);
        SelectableComponent *selectable = getComponent(entity, SelectableComponent);
        selectable->selected = false;
        plistSet(&selectedEntities, i, nullptr);
    }
    selectedEntitiesCount = 0;
}

static void stop()
{
    clearSelectedEntities();
    plistFree(&selectedEntities);
}

static void updateSelectionRecangle()
{
    float downX = mouseDownPoint.x;
    float downY = mouseDownPoint.y;
    float dragX = mouseDragPoint.x;
    float dragY = mouseDragPoint.y;

    selectionRectangle.x = MIN(downX, dragX);
    selectionRectangle.y = MIN(downY, dragY);
    selectionRectangle.width = fabsf(downX - dragX);
    selectionRectangle.height = fabsf(downY - dragY);
}

static void checkForSingleSelection()
{
    LList units;
    Rc rc = prayEntityLookupAll(&units, C(CID(SelectableComponent), CID(Transform2DComponent), CID(Collider2DComponent)), 3);
    if (rc != RC_OK)
    {
        return;
    }

    LNode *node = nullptr;
    LListForEach(&units, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        auto transform = getComponent(entity, Transform2DComponent);
        auto collider2D = getComponent(entity, Collider2DComponent);
        auto selectable = getComponent(entity, SelectableComponent);

        if (CheckCollisionPointCircle(mouseDownPoint, transform->position, collider2D->radius))
        {
            selectable->selected = true;
            addSelectedEntity(entity);
            break;
        }
    }
}

static void checkForMultiSelection()
{
    LList units;
    Rc rc = prayEntityLookupAll(&units, C(CID(SelectableComponent), CID(Transform2DComponent), CID(Collider2DComponent)), 3);
    if (rc != RC_OK)
    {
        return;
    }

    LNode *node = nullptr;
    LListForEach(&units, node)
    {
        Entity *entity = LListGetEntry(node, Entity);
        auto transform = getComponent(entity, Transform2DComponent);
        auto collider2D = getComponent(entity, Collider2DComponent);
        auto selectable = getComponent(entity, SelectableComponent);

        Vector2 position = prayVector2Add(transform->position, collider2D->offset);
        if (CheckCollisionCircleRec(position, collider2D->radius, selectionRectangle))
        {
            selectable->selected = true;
            addSelectedEntity(entity);
        }
    }
}

static void gameUpdate()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        mouseDownPoint = GetScreenToWorld2D(GetMousePosition(), *prayGetCamera());

        clearSelectedEntities();

        checkForSingleSelection();
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        mouseDragPoint = GetScreenToWorld2D(GetMousePosition(), *prayGetCamera());

        updateSelectionRecangle();

        checkForMultiSelection();
    }
}

static void renderScreenSpace()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        DrawRectangle((int) selectionRectangle.x,
                      (int) selectionRectangle.y,
                      (int) selectionRectangle.width,
                      (int) selectionRectangle.height,
                      selectionColor);
        DrawRectangleLinesEx(selectionRectangle, 2.0f, selectionBorderColor);
    }
}

void registerSelectionSystem()
{
    System system = {
        .name = "Selection System",
        .start = start,
        .stop = stop,
        .gameUpdate = gameUpdate,
        .renderUpdateWorldSpace = renderScreenSpace,
    };
    praySystemsRegister(system);
}

void selectionGetSelectedEntities(PList *selected)
{
    memcpy(selected, &selectedEntities, sizeof(PList));
    selected->length = selectedEntitiesCount;
}


bool selectionEntitiesSelected()
{
    return selectedEntitiesCount != 0;
}
