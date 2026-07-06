#ifndef GFX_GFX_H
#define GFX_GFX_H

#include <stdint.h>

#define GFX_W      16
#define GFX_HEIGHT 16
#define GFX_BYTES 128

#define COLOR_TRANSPARENT 0x0
#define COLOR_SKY         0x2

void gfx_draw_indexed(int16_t sx, int16_t sy, const uint8_t *data, uint8_t flip);
uint8_t gfx_last_opaque_row(const uint8_t *data);
void gfx_draw_indexed_at_feet(int16_t sx, int16_t feet_y, const uint8_t *data, uint8_t flip);

#endif
