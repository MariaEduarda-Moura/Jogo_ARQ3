#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include "hw_regs.h"

#define FB_WIDTH   256
#define FB_HEIGHT  240
#define FB_PIXELS  (FB_WIDTH * FB_HEIGHT)

void fb_init(void);
void fb_set_addr(uint32_t addr);
void fb_write_pixel(uint8_t color);
void fb_write_at(uint16_t x, uint16_t y, uint8_t color);
void fb_clear(uint8_t color);
void fb_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
void fb_present(void);

#endif
