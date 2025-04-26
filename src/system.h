
#ifndef SYSTEM_H
#define SYSTEM_H

#include "array_list.h"

typedef struct
{
    char systemName[50];
    bool initialized;
    bool started;
    void (*start)(void);
    void (*close)(void);
    void (*gameUpdate)(void);
    void (*renderUpdate)(void);
} System;

void systemNoop(void);
System *systemNew();
System *systemFree(System *system);
void systemsInit();
void systemsDestroy();
AList *systemsGet();
Rc systemsAdd(System *system);
Rc systemsRemove(System *system);

#endif
