#include "render.h"
#include "game.h"
#include "tiles.h"
#include "tile_gfx.h"
#include "sprites.h"
#include "enemy.h"
#include "collision.h"
#include "gfx.h"
#include "hw/framebuffer.h"
#include <sleep.h>

void render_init(void)
{
    fb_init();
}

void render_frame(void)
{
    int32_t tx, ty;
    int32_t start_tx = camera.x / TILE_SIZE;
    int32_t end_tx = start_tx + (FB_WIDTH / TILE_SIZE) + 2;

    fb_clear(COLOR_SKY);

    for (ty = 0; ty < LEVEL_HEIGHT; ty++) {
        for (tx = start_tx; tx < end_tx && tx < LEVEL_WIDTH; tx++) {
            uint8_t tile = level_map[ty * LEVEL_WIDTH + tx];
            if (tile == TILE_EMPTY || tile >= 8)
                continue;
            gfx_draw_indexed(
                (int16_t)(tx * TILE_SIZE - camera.x),
                (int16_t)(ty * TILE_SIZE),
                tile_gfx_table[tile],
                0);
        }
    }

    enemy_render();
    gfx_draw_indexed_at_feet(
        (int16_t)(mario.x - camera.x),
        (int16_t)(mario.y + MARIO_HITBOX_H),
        sprite_for_state((uint8_t)mario.state, mario.anim_frame),
        mario.facing_left);
    usleep(16666);
    fb_present();
}
