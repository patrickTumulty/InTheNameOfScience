
#include "astar.h"
#include "linked_list.h"
#include "utils.h"
#include <raylib.h>
#include <stdint.h>


typedef struct {
    Vector2 position;
    float f;
    float g;
    float h;
} AStarCell;

/**
 * vector within bounds of navGrid
 */
static bool inBounds(Vector2 vec, uint32_t xMax, uint32_t yMax)
{
    return (0 <= vec.x && vec.x < xMax) && (0 <= vec.y && vec.y < yMax);
}

static bool isUnBlocked(Vector2 vec, bool **navGrid)
{
    return navGrid[(int) vec.y][(int) vec.x];
}

void astar(Vector2 start,
           Vector2 dest,
           bool **navGrid,
           uint32_t rows,
           uint32_t cols)
{
    if (!inBounds(start, rows, cols) && !inBounds(dest, rows, cols))
    {
        return;
    }

    if (!isUnBlocked(start, navGrid) && !isUnBlocked(dest, navGrid))
    {
        return;
    }

    if (VEC2EQ(start, dest))
    {
        return; // Already there
    }


    LList llist = { 0 };

    LNode *node;
    int *num;

    LListForEach(&llist, node) {
        num = LListGetEntry(node, int);
    }



}


