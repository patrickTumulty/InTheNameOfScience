
#include "common_utils.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <float.h>

void hexdump(uint8_t *buf, uint32_t bufLen)
{
    constexpr int cols = 16;
    int rows = bufLen / cols;
    if (bufLen % 16 != 0) rows++;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int index = (i * 16) + j;
            char repr[4] = {0};
            if (index < bufLen)
            {
                snprintf(repr, sizeof(repr), "%02X ", buf[index]);
            }
            else
            {
                snprintf(repr, sizeof(repr), "   ");
            }
            printf("%s", repr);
        }
        printf("\n");
    }
}

bool inBounds(int v, int min, int max)
{
    return min <= v && v < max;
}

const char *rc2str(Rc rc)
{
    switch (rc)
    {
        case RC_OK: return "OK";
        case RC_ERROR: return "ERROR";
        case RC_MEM_ALLOC_ERROR: return "MEM_ALLOC_ERROR";
        case RC_BAD_PARAM: return "BAD_PARAM";
        case RC_OUT_OF_BOUNDS: return "OUT_OF_BOUNDS";
        case RC_NOT_FOUND: return "NOT_FOUND";
        default: return "UNKNOWN";
    }
}


bool feq(float f1, float f2)
{
    return fabs(f1 - f2) < FLT_EPSILON;
}
