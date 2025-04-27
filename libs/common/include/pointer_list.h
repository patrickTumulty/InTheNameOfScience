
#ifndef POINTER_LIST_H
#define POINTER_LIST_H

#include "array_list.h"

typedef AList PList;

Rc plistNew(PList *plist, u32 length);
Rc plistAppend(PList *plist, const void *ptr);
Rc plistResize(PList *plist, u32 length);
void *plistGet(PList *plist, int index);
Rc plistSet(PList *plist, int index, const void *ptr);
u32 plistLength(PList *plist);
void plistFree(PList *plist);

#endif // POINTER_LIST_H
