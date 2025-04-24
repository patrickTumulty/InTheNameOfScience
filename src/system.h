
#ifndef SYSTEM_H
#define SYSTEM_H

typedef struct {
    void (*start)(void);
    void (*close)(void);
    void (*gameUpdate)(void);
    void (*renderUpdate)(void);
} System;

#endif
