
#include "astar.h"
#include "bool_mat.h"
#include "linked_list.h"
#include "utils.h"
#include <stdio.h>

struct ListNumber
{
    int num;
    LNode node;
};

int main(void)
{
    Position start = {10, 10};
    Position dest = {15, 15};

    BoolMat *navGrid = boolMatNew(30, 30, true);
    AStarPath path = {0};

    astar(start, dest, navGrid, &path);

    printf("path=%d\n", path.pathLen);

    for (int i = 0; i < path.pathLen; i++)
    {
        printf("x=%2d, y=%2d\n", path.path[i].x, path.path[i].y);
    }
}
