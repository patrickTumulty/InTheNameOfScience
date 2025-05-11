
#include "health_component.h"


void initHealth(void *component)
{
    HealthComponent *healthComponent = component;
    healthComponent->maxHealth = 100;
    healthComponent->currentHealth = 100;
}
