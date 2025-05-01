
#include "game_math.h"
#include "raylib.h"
#include <math.h>


/**
 * @brief Calculate angle between 2 points 
 *
 * @param[[in]] p1 point 1 
 * @param[[in]] p2 point 2 
 *
 * @return angle in degrees
 */
float calculateAngle(Vector2 p1, Vector2 p2)
{
    float deltaX = p1.x - p2.x;
    float deltaY = p1.y - p2.y;
    return RAD2DEG * atan2f(deltaY, deltaX);
}

/**
 * @brief calculate a point on a circle around a point 
 *
 * @param[[in]] origin origin point  
 * @param[[in]] radians degrees in radians 
 * @param[[in]] radius radius 
 *
 * @return point on circle 
 */
Vector2 calculateRotation(Vector2 origin, float radians, float radius)
{
    float x = radius * cosf(radians);
    float y = radius * sinf(radians);
    return (Vector2) {
        .x = x + origin.x,
        .y = y + origin.y,
    };
}
