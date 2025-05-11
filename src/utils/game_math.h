
#ifndef GAME_MATH_H
#define GAME_MATH_H

#include "raylib.h"

#define POS2VEC(POS) \
    (Vector2) { .x = (POS).x, .y = (POS).y }
#define VEC2POS(VEC) \
    (Position) { .x = (VEC).x, .y = (VEC).y }

void calculateTriangle(Vector2 center, float roatationDegrees, float radius, Vector2 points[3]);
float calculateAngle(Vector2 p1, Vector2 p2);
Vector2 calculatePointOnCircle(Vector2 origin, float radians, float radius);

float calculateSlope(Vector2 p1, Vector2 p2);
float vector2Distance(Vector2 p1, Vector2 p2);

#endif // GAME_MATH_H
