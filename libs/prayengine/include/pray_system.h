
#ifndef SYSTEM_H
#define SYSTEM_H

#include "array_list.h"

typedef struct
{
    char name[50];
    void (*start)(void);
    void (*stop)(void);
    void (*gameUpdate)(void);
    void (*renderUpdateWorldSpace)(void);
    void (*renderUpdateScreenSpace)(void);
} System;

void praySystemNoop(void);
void praySystemsInit();
void praySystemsDestroy();
AList *praySystemsGetList();

Rc praySystemsRegister(System system);

void praySystemsRunStart();
void praySystemsRunStop();
void praySystemsRunGameUpdate();
void praySystemsRunRenderUpdateWorldSpace();
void praySystemsRunRenderUpdateScreenSpace();

#endif
