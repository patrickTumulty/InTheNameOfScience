
#include "world_system.h"
#include "bool_mat.h"
#include "components.h"
#include "entity.h"
#include "entity_registry.h"
#include "pray_colors.h"
#include "pray_globals.h"
#include "system.h"
#include "tmem.h"

// static BoolMat *navGrid;
// static char world[WORLD_HEIGHT][WORLD_WIDTH] = {0};
// static Position startingPoint = {1, 1};
//
static Entity *worldEntity;
static World *worldComponent;

static void start()
{
    ComponentID cids[] = {CID_TRANSFORM, CID_WORLD};

    worldEntity = entityNew(cids, 2);

    worldComponent = entityGetComponent(worldEntity, CID_WORLD);
    worldComponent->navGrid = boolMatNew(WORLD_HEIGHT, WORLD_WIDTH, true, false);
    worldComponent->rows = WORLD_HEIGHT;
    worldComponent->cols = WORLD_HEIGHT;

    u64 rows = worldComponent->rows;
    u64 cols = worldComponent->cols;

    worldComponent->world = (char **) tmemcalloc(1, (u64) (rows * sizeof(char *)) + (rows * cols * sizeof(char)));
    char *data = ((char *) worldComponent->world) + (sizeof(char *) * rows);
    for (u64 i = 0; i < rows; i++)
    {
        worldComponent->world[i] = (data + i * cols);
    }

    entityRegistryRegister(worldEntity);
}

static void stop()
{
    tmemfree((void *) worldComponent->world);
    boolMatFree(worldComponent->navGrid);

    entityFree(worldEntity);
}

static void renderWorld()
{
    for (int i = 0; i < WORLD_HEIGHT; i++)
    {
        for (int j = 0; j < WORLD_WIDTH; j++)
        {
            Color color;

            if (boolMatGet(worldComponent->navGrid, j, i) == 0)
            {
                color = BLACK;
            }
            else if (worldComponent->world[i][j] == '1')
            {
                color = RED;
            }
            else if (worldComponent->world[i][j] == '2')
            {
                color = GREEN;
            }
            else if (worldComponent->world[i][j] == '3')
            {
                color = BLUE;
            }
            else
            {
                if (i % 2 == 0)
                {
                    color = (j % 2 == 0) ? MATERIAL_BLUE_GREY_100 : MATERIAL_BLUE_GREY_200;
                }
                else
                {
                    color = (j % 2 == 0) ? MATERIAL_BLUE_GREY_200 : MATERIAL_BLUE_GREY_100;
                }
            }
            DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
        }
    }

    Rectangle rect;
    rect.height = TILE_SIZE * WORLD_HEIGHT;
    rect.width = TILE_SIZE * WORLD_WIDTH;
    rect.x = 0;
    rect.y = 0;
    DrawRectangleLinesEx(rect, 3.0f, MATERIAL_BLUE_GREY_700);
}

void registerWorldSystem()
{
    System *system = systemNew();
    system->start = start;
    system->stop = stop;
    system->renderUpdateWorldSpace = renderWorld;
}
