
#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

typedef void(*OnHealthDepletedCB)(void);

typedef struct
{
    OnHealthDepletedCB healthDepletedCallback;
    int maxHealth;
    int currentHealth;
} HealthComponent;

void initHealth(void *component);

#endif // HEALTH_COMPONENT_H
