
#ifndef PRAY_UTILS_H
#define PRAY_UTILS_H

#include "raylib.h"

Vector2 prayVector2MoveTowards(Vector2 current, Vector2 target, float maxDistanceDelta);
Vector2 prayVector2Add(Vector2 v1, Vector2 v2);
bool prayVector2Equals(Vector2 v1, Vector2 v2);

#endif // PRAY_UTILS_H
