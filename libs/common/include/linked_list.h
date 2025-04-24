
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>
#include <stdint.h>

struct LNode;

typedef struct LNode {
    void *data;
    struct LNode *next;
    struct LNode *prev;
} LNode;

typedef struct {
    LNode *head;
    LNode *tail;
    uint32_t size;
} LList;

#define LListForEach(LL, IT) for ((IT) = (LL)->head; (IT) != NULL; (IT) = (IT)->next)  

#define LListGetEntry(NODE, T) ((T *) (NODE)->data)

void llistInit(LList *llist);
void llistAppend(LList *llist, LNode *lnode);
void llistInsert(LList *llist, uint32_t index, LNode *lnode);
void llistPushFront(LList *llist, LNode *lnode);
void llistPushBack(LList *llist, LNode *lnode);
LNode *llistPopBack(LList *llist);
LNode *llistPopFront(LList *llist);
LNode *llistRemove(LList *llist, LNode *lnode);
void llistInitNode(LNode *lnode, void *data);
bool llistIsEmpty(LList *llist);

#endif // LINKED_LIST_H
