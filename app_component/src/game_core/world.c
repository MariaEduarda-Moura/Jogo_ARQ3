#include "world.h"
#include "tiles.h"

uint8_t world_tile_solid(uint8_t tile_id)
{
    switch (tile_id) {
    case TILE_GROUND:
    case TILE_BRICK:
    case TILE_BLOCK:
    case TILE_PIPE_TOP:
    case TILE_PIPE_BODY:
        return 1;
    default:
        return 0;
    }
}

uint8_t world_tile_at(int32_t world_x, int32_t world_y)
{
    int32_t tx, ty;

    if (world_x < 0 || world_y < 0)
        return TILE_EMPTY;
    tx = world_x / TILE_SIZE;
    ty = world_y / TILE_SIZE;
    if (tx >= LEVEL_WIDTH || ty >= LEVEL_HEIGHT)
        return TILE_EMPTY;
    return level_map[ty * LEVEL_WIDTH + tx];
}
