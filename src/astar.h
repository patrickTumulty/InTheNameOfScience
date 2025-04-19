
#ifndef ASTAR_H
#define ASTAR_H

#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>

void astar(Vector2 initialPosition, Vector2 targetPosition, bool **navGrid, uint32_t rows, uint32_t cols);

#endif// ASTAR_H
