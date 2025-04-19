
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>
#include <stdint.h>



struct LNode;

typedef struct LNode {
    void *data;
    size_t dataLen;
    struct LNode *next;
    struct LNode *prev;
} LNode;

typedef struct {
    LNode *head;
    uint32_t size;
} LList;


#define LListForEach(LL, IT) for ((IT) = (LL)->head; (IT) != NULL; (IT) = (IT)->next)  

#define LListGetEntry(NODE, T) ((T *) (NODE)->data)

void llistInit(LList *llist);
void llistAppend(LList *llist, LNode *lnode);
void llistRemove(LList *llist, LNode *lnode);
void llistInitNode(LNode *lnode, void *data);

#endif // LINKED_LIST_H
