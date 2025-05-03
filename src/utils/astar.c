
#include "astar.h"
#include "bool_mat.h"
#include "linked_list.h"
#include "tmem.h"
#include "common_types.h"
#include "common_utils.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
    Position position;
    Position parent;
    uint32_t f; // g + h
    uint32_t g; // Distance from starting node
    uint32_t h; // Distance from destination node
    LNode lnode;
} AStarCell;

typedef struct
{
    Position pos;
    uint32_t cost;
} AStarMove;

constexpr static int DIST = 10;
constexpr static int DISTD = 14;

constexpr static int MOVES = 8;
constexpr static AStarMove moves[] = {
    {  {1, 0},  DIST},
    {  {0, 1},  DIST},
    { {-1, 0},  DIST},
    { {0, -1},  DIST},
    {  {1, 1}, DISTD},
    { {1, -1}, DISTD},
    { {-1, 1}, DISTD},
    {{-1, -1}, DISTD},
};

typedef enum
{
    HEURISTIC_MANHATTAN,
    HEURISTIC_DIAGONAL,
    HEURISTIC_EUCLIDIAN,
} AStarHeuristic;

constexpr static AStarHeuristic DEFAULT_HEURISTIC = HEURISTIC_EUCLIDIAN;

static int heuristicManhattan(Position p, Position dest)
{
    return abs(p.x - dest.x) + abs(p.y - dest.y);
}

static int heuristicDiagonalDistance(Position p, Position dest)
{
    int dx = abs(p.x - dest.x);
    int dy = abs(p.y - dest.y);
    return (DIST * (dx + dy)) + ((DISTD - 2 * DIST) * MIN(dx, dy));
}

static int heuristicEuclidianDistance(Position p, Position dest)
{
    int dx = p.x - dest.x;
    int dy = p.x - dest.x;
    return (int) sqrt((dx * dx) - (dy * dy));
}

typedef int (*CalcHDist)(Position p, Position dest);

static CalcHDist getHeuriticsFunction(AStarHeuristic hFunc)
{
    switch (hFunc)
    {
        case HEURISTIC_MANHATTAN:
            return heuristicManhattan;
        case HEURISTIC_DIAGONAL:
            return heuristicDiagonalDistance;
        case HEURISTIC_EUCLIDIAN:
        default:
            return heuristicEuclidianDistance;
    }
}

static AStarCell **newAStarCellMat(uint32_t rows, uint32_t cols)
{
    uint8_t *data = tmemcalloc(1, (sizeof(AStarCell *) * rows) + (sizeof(AStarCell) * rows * cols));
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
            cell->f = UINT32_MAX;
            cell->g = UINT32_MAX;
            cell->h = UINT32_MAX;
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
    return !boolMatGet(navGrid, pos.x, pos.y);
}

static void tracePath(AStarCell *destCell, AStarCell **cellMat, AStarPath *path)
{
    LList tracePath = {};

    llistInit(&tracePath);

    AStarCell *cell = destCell;
    llistAppend(&tracePath, &cell->lnode);

    while (!POSEQ(cell->position, cell->parent))
    {
        cell = &cellMat[cell->parent.y][cell->parent.x];
        llistAppend(&tracePath, &cell->lnode);
    }

    path->path = tmemcalloc(1, sizeof(Position) * tracePath.size);

    LNode *node = nullptr;
    uint32_t index = tracePath.size - 1;
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

    CalcHDist calcHDist = getHeuriticsFunction(DEFAULT_HEURISTIC);
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

            uint32_t g = p->g + move->cost;
            uint32_t h = calcHDist(newPos, destPos);
            uint32_t f = g + h;

            if (cell->f == UINT32_MAX || cell->f > f)
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
    tmemfree((void*)cellMat);
}
