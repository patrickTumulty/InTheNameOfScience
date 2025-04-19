
#include "linked_list.h"
#include <stdlib.h>

void llistInit(LList *llist)
{
    llist->head = NULL;
    llist->size = 0;
}

static LNode *llistGetTail(LNode *node)
{
    if (node->next == NULL)
    {
        return node;
    }
    return llistGetTail(node->next);
}

void llistInitNode(LNode *node, void *data)
{
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
}

void llistAppend(LList *llist, LNode *lnode)
{
    if (llist->head == NULL)
    {
        llist->head = lnode;
        return;
    }

    LNode *tail = llistGetTail(llist->head);
    tail->next = lnode;
    lnode->prev = tail;
    llist->size++;
}

void llistRemove(LList *llist, LNode *lnode)
{
    if (lnode == NULL || llist == NULL)
    {
        return;
    }

    if (lnode->next != NULL)
    {
        lnode->next->prev = lnode->prev;
    }

    if (lnode->prev != NULL)
    {
        lnode->prev->next = lnode->next;
    }
}
