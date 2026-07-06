#include "gfx.h"
#include "hw/framebuffer.h"

static uint8_t gfx_get_pixel(const uint8_t *data, uint8_t x, uint8_t y)
{
    uint16_t idx = (uint16_t)y * GFX_W + x;
    uint16_t byte_idx = idx >> 1;

    if (idx & 1u)
        return data[byte_idx] & 0x0Fu;
    return data[byte_idx] >> 4;
}

uint8_t gfx_last_opaque_row(const uint8_t *data)
{
    int16_t row;

    for (row = GFX_HEIGHT - 1; row >= 0; row--) {
        uint8_t col;

        for (col = 0; col < GFX_W; col++) {
            if (gfx_get_pixel(data, col, (uint8_t)row) != COLOR_TRANSPARENT)
                return (uint8_t)row;
        }
    }
    return 0;
}

void gfx_draw_indexed(int16_t sx, int16_t sy, const uint8_t *data, uint8_t flip)
{
    uint8_t row, col;

    for (row = 0; row < GFX_HEIGHT; row++) {
        for (col = 0; col < GFX_W; col++) {
            uint8_t src_col = flip ? (uint8_t)(GFX_W - 1 - col) : col;
            uint8_t color = gfx_get_pixel(data, src_col, row);
            int16_t px = sx + col;
            int16_t py = sy + row;

            if (color == COLOR_TRANSPARENT)
                continue;
            if (px < 0 || py < 0 || px >= FB_WIDTH || py >= FB_HEIGHT)
                continue;
            fb_write_at((uint16_t)px, (uint16_t)py, color);
        }
    }
}

void gfx_draw_indexed_at_feet(int16_t sx, int16_t feet_y, const uint8_t *data, uint8_t flip)
{
    uint8_t feet_row = gfx_last_opaque_row(data);

    gfx_draw_indexed(sx, (int16_t)(feet_y - feet_row), data, flip);
}
