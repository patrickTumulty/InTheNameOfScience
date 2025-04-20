
#include "linked_list.h"
#include <stdlib.h>

void llistInit(LList *llist)
{
    llist->head = NULL;
    llist->size = 0;
}

void llistInitNode(LNode *node, void *data)
{
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
}

bool llistIsEmpty(LList *llist)
{
    return llist->head == NULL;
}

void llistAppend(LList *llist, LNode *lnode)
{
    if (llist->head == NULL)
    {
        llist->head = lnode;
        llist->tail = lnode;
        llist->size = 1;
        return;
    }

    LNode *tail = llist->tail;
    tail->next = lnode;
    lnode->prev = tail;
    llist->tail = lnode;
    llist->size++;
}

LNode *llistRemove(LList *llist, LNode *lnode)
{
    if (lnode == NULL || llist == NULL)
    {
        return NULL;
    }

    if (lnode->next != NULL)
    {
        lnode->next->prev = lnode->prev;
    }

    if (llist->head == lnode)
    {
        llist->head = lnode->next;
    }

    if (lnode->prev != NULL)
    {
        lnode->prev->next = lnode->next;
    }

    if (llist->tail == lnode)
    {
        llist->tail = lnode->prev;
    }

    lnode->next = NULL;
    lnode->prev = NULL;

    llist->size--;

    return lnode;
}


void llistPushFront(LList *llist, LNode *lnode)
{
    if (llist->head == NULL)
    {
        llist->head = lnode;
        llist->tail = lnode;
        return;
    }

    LNode *head = llist->head;
    head->prev = lnode;
    lnode->next = head;
    llist->head = lnode;
    llist->size++;
}

void llistPushBack(LList *llist, LNode *lnode)
{
    if (llist->head == NULL)
    {
        llist->head = lnode;
        llist->tail = lnode;
        return;
    }

    LNode *tail = llist->tail;
    tail->next = lnode;
    lnode->prev = tail;
    llist->tail = lnode;
    llist->size++;
}

LNode *llistPopBack(LList *llist)
{
    if (llist->tail == NULL)
    {
        return NULL;
    }
    LNode *tail = llist->tail;
    llist->tail = tail->prev;
    llist->tail->next = NULL;
    llist->size--;
    return tail;
}

LNode *llistPopFront(LList *llist)
{
    if (llist->head == NULL)
    {
        return NULL;
    }
    LNode *head = llist->head;
    llist->head = head->next;
    llist->head->prev = NULL;
    llist->size--;
    return head;
}
