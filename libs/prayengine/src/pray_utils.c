
#include "pray_utils.h"
#include "common_utils.h"
#include <math.h>


Vector2 prayVector2MoveTowards(Vector2 current, Vector2 target, float maxDistanceDelta)
{
    float deltaX = target.x - current.x;
    float deltaY = target.y - current.y;

    float sqDist = (deltaX * deltaX) + (deltaY * deltaY);

    if (feq(sqDist, 0.0f) || (maxDistanceDelta >= 0 && sqDist <= maxDistanceDelta * maxDistanceDelta))
    {
        return target;
    }

    float dist = sqrtf(sqDist);
    return (Vector2) {
        .x = current.x + (deltaX / dist * maxDistanceDelta),
        .y = current.y + (deltaY / dist * maxDistanceDelta),
    };
}

Vector2 prayVector2Add(Vector2 v1, Vector2 v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

bool prayVector2Equals(Vector2 v1, Vector2 v2)
{
    return feq(v1.x, v2.x) && feq(v1.y, v2.y);
}
