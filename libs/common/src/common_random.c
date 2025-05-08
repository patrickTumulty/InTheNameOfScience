
#include "common_random.h"
#include <stdlib.h>


float randomFloat(float min, float max)
{
    float normalized = (float) random() / (float) RAND_MAX;
    return min + (normalized * (max - min));
}
