
#ifndef GAME_MATH_H
#define GAME_MATH_H

#include "raylib.h"

float calculateAngle(Vector2 p1, Vector2 p2);
Vector2 calculatePointOnCircle(Vector2 origin, float radians, float radius);

float calculateSlope(Vector2 p1, Vector2 p2);

#endif // GAME_MATH_H
