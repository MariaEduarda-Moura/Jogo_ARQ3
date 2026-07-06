#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>
#include "game.h"

uint8_t world_tile_at(int32_t world_x, int32_t world_y);
uint8_t world_tile_solid(uint8_t tile_id);

#endif
