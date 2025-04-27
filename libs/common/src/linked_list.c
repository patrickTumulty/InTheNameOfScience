
#include "linked_list.h"
#include <stdbool.h>

static bool llistContainsNode(LList *llist, LNode *lnode)
{
    LNode *n = nullptr;
    LListForEach(llist, n)
    {
        if (n == lnode)
        {
            return true;
        }
    }
    return false;
}

void llistInit(LList *llist)
{
    llist->head = nullptr;
    llist->tail = nullptr;
    llist->size = 0;
}

void llistInitNode(LNode *node, void *data)
{
    node->data = data;
    node->next = nullptr;
    node->prev = nullptr;
}

bool llistIsEmpty(LList *llist)
{
    return llist->head == nullptr;
}

void llistAppend(LList *llist, LNode *lnode)
{
    llistPushBack(llist, lnode);
}

LNode *llistRemove(LList *llist, LNode *lnode)
{
    if (lnode == nullptr || llist == nullptr)
    {
        return nullptr;
    }

    if (!llistContainsNode(llist, lnode))
    {
        return nullptr;
    }

    if (lnode->next != nullptr)
    {
        lnode->next->prev = lnode->prev;
    }

    if (llist->head == lnode)
    {
        llist->head = lnode->next;
    }

    if (lnode->prev != nullptr)
    {
        lnode->prev->next = lnode->next;
    }

    if (llist->tail == lnode)
    {
        llist->tail = lnode->prev;
    }

    lnode->next = nullptr;
    lnode->prev = nullptr;

    llist->size--;

    return lnode;
}


void llistPushFront(LList *llist, LNode *lnode)
{
    if (llistContainsNode(llist, lnode))
    {
        return;
    }

    if (llist->head == nullptr)
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
    if (llistContainsNode(llist, lnode))
    {
        return;
    }

    if (llist->head == nullptr)
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

LNode *llistPopBack(LList *llist)
{
    if (llist->tail == nullptr)
    {
        return nullptr;
    }
    LNode *tail = llist->tail;
    llist->tail = tail->prev;
    if (llist->tail != nullptr)
    {
        llist->tail->next = nullptr;
    }
    llist->size--;
    return tail;
}

LNode *llistPopFront(LList *llist)
{
    if (llist->head == nullptr)
    {
        return nullptr;
    }
    LNode *head = llist->head;
    llist->head = head->next;
    if (llist->head != nullptr)
    {
        llist->head->prev = nullptr;
    }
    llist->size--;
    return head;
}
