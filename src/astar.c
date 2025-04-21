
#include "astar.h"
#include "bool_mat.h"
#include "linked_list.h"
#include "tmem.h"
#include "utils.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct
{
    Position parent;
    Position position;
    float f; // g + h
    float g; // Distance from starting node
    float h; // Distance from destination node
    LNode lnode;
} AStarCell;

typedef struct
{
    Position pos;
    int cost;
} AStarMove;

constexpr int MOVES = 8;
constexpr AStarMove moves[] = {
    {{1, 0}, 10},
    {{0, 1}, 10},
    {{-1, 0}, 10},
    {{0, -1}, 10},
    {{1, 1}, 14},
    {{1, -1}, 14},
    {{-1, 1}, 14},
    {{-1, -1}, 14},
};

AStarCell **newAStarCellMat(int rows, int cols)
{
    int s = (sizeof(AStarCell *) * rows) + (sizeof(AStarCell) * rows * cols);
    uint8_t *data = calloc(1, s);
    if (data == nullptr)
    {
        return nullptr;
    }

    uint32_t ptrOffset = sizeof(AStarCell *) * rows;
    uint32_t dataOffset = 0;
    AStarCell **cellMat = (AStarCell **) data;

    for (int i = 0; i < rows; i++)
    {
        cellMat[i] = (AStarCell *) (data + ptrOffset + dataOffset);
        dataOffset += sizeof(AStarCell) * cols;
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            AStarCell *cell = &cellMat[i][j];
            cell->position.x = j;
            cell->position.y = i;
            cell->parent.x = -1;
            cell->parent.y = -1;
            cell->f = INFINITY;
            cell->g = INFINITY;
            cell->h = INFINITY;
            llistInitNode(&cell->lnode, cell);
        }
    }

    return cellMat;
}

static bool outOfBounds(Position pos, uint32_t xMax, uint32_t yMax)
{
    return !((0 <= pos.x && pos.x < xMax) && (0 <= pos.y && pos.y < yMax));
}

static bool isClosed(const BoolMat *closedMat, Position pos)
{
    return boolMatGet(closedMat, pos.x, pos.y);
}

static bool isBlocked(const BoolMat *navGrid, Position pos)
{
    return boolMatGet(navGrid, pos.x, pos.y) == false;
}

static float calcHVal(Position p, Position dest)
{
    float px = p.x - dest.x;
    float py = p.y - dest.y;
    return sqrtf((px * px) + (py * py));
}

static void tracePath(AStarCell *destCell, AStarCell **cellMat, AStarPath *path)
{
    LList tracePath = {0};

    llistInit(&tracePath);

    AStarCell *cell = destCell;
    llistAppend(&tracePath, &cell->lnode);

    while (!POSEQ(cell->position, cell->parent))
    {
        cell = &cellMat[cell->parent.y][cell->parent.x];
        llistAppend(&tracePath, &cell->lnode);
    }

    path->path = calloc(1, sizeof(Position) * tracePath.size);

    LNode *node = nullptr;
    int index = tracePath.size - 1;
    LListForEach(&tracePath, node)
    {
        AStarCell *cell = LListGetEntry(node, AStarCell);
        Position *pos = &path->path[index--];
        pos->x = cell->parent.x;
        pos->y = cell->parent.y;
    }

    path->pathLen = tracePath.size;
}

void astar(Position startPos, Position destPos, const BoolMat *navGrid, AStarPath *path)
{
    if (outOfBounds(startPos, navGrid->rows, navGrid->cols) ||
        outOfBounds(destPos, navGrid->rows, navGrid->cols))
    {
        return;
    }

    if (isBlocked(navGrid, startPos) || isBlocked(navGrid, destPos))
    {
        return;
    }

    if (POSEQ(startPos, destPos))
    {
        return; // Already there
    }

    BoolMat *closedMat = boolMatNew(navGrid->rows, navGrid->cols, false, true);
    AStarCell **cellMat = newAStarCellMat(navGrid->rows, navGrid->cols);

    AStarCell *startingCell = &cellMat[startPos.y][startPos.x];
    startingCell->f = 0;
    startingCell->g = 0;
    startingCell->h = 0;
    startingCell->parent = startingCell->position;

    LList openList = {0};
    llistInit(&openList);

    llistAppend(&openList, &startingCell->lnode);

    while (!llistIsEmpty(&openList))
    {
        AStarCell *p = nullptr;
        LNode *node = nullptr;
        LListForEach(&openList, node)
        {
            AStarCell *q = LListGetEntry(node, AStarCell);
            if (p == nullptr)
            {
                p = q;
                continue;
            }

            if (q->f < p->f)
            {
                p = q;
            }
        }

        llistRemove(&openList, &p->lnode);

        Position pos = p->position;

        boolMatSet(closedMat, pos.x, pos.y, true);

        for (int i = 0; i < MOVES; i++)
        {
            const AStarMove *move = &moves[i];

            Position newPos = {
                .x = pos.x + move->pos.x,
                .y = pos.y + move->pos.y,
            };

            if (outOfBounds(newPos, navGrid->cols, navGrid->rows))
            {
                continue;
            }

            AStarCell *cell = &cellMat[newPos.y][newPos.x];

            if (POSEQ(newPos, destPos))
            {
                cell->parent = pos;
                tracePath(cell, cellMat, path);
                goto EXIT;
            }

            if (isClosed(closedMat, newPos) || isBlocked(navGrid, newPos))
            {
                continue;
            }

            float g = p->g + move->cost;
            float h = calcHVal(newPos, destPos);
            float f = g + h;

            if (cell->f == INFINITY || cell->f > f)
            {
                llistAppend(&openList, &cell->lnode);
                cell->f = f;
                cell->h = h;
                cell->g = g;
                cell->parent = pos;
            }
        }
    }

EXIT:

    closedMat = boolMatFree(closedMat);
    free(cellMat);
}
