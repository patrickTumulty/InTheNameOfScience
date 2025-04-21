
#include "utils.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


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
