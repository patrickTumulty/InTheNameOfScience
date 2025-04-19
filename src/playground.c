
#include "linked_list.h"
#include <stdio.h>

struct ListNumber
{
    int num;
    LNode node;
};

int main(void)
{
    LList llist;

    llistInit(&llist);

    struct ListNumber numbers[] = {
        {.num = 0, .node = {0}},
        {.num = 1, .node = {0}},
        {.num = 2, .node = {0}},
        {.num = 3, .node = {0}},
    };

    for (int i = 0; i < 4; i++) {
        llistInitNode(&numbers[i].node, &numbers[i]);
        llistAppend(&llist, &numbers[i].node);
    }

    llistRemove(&llist, &numbers[2].node);

    struct ListNumber *num;
    LNode *node;

    LListForEach(&llist, node) {
        num = LListGetEntry(node, struct ListNumber);
        printf("%d\n", num->num);
    }

}
