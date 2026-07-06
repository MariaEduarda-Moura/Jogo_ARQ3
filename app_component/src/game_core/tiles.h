#ifndef TILES_H
#define TILES_H

#include <stdint.h>
#include "game.h"

enum {
    TILE_EMPTY = 0,
    TILE_GROUND,
    TILE_BRICK,
    TILE_BLOCK,
    TILE_PIPE_TOP,
    TILE_PIPE_BODY,
    TILE_CLOUD,
    TILE_BUSH
};

extern const uint8_t level_map[LEVEL_WIDTH * LEVEL_HEIGHT];

#endif
