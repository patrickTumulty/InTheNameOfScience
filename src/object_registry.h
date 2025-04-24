
#ifndef OBJECT_REGISTRY_H
#define OBJECT_REGISTRY_H

#include "array_list.h"
#include "common_types.h"
#include "linked_list.h"
#include <raylib.h>
#include <stdint.h>
#include "components.h"

void initObjectRegistry();
void destroyObjectRegistry();


Rc registerComponent(ComponentID id, u64 componentSize);

#endif
