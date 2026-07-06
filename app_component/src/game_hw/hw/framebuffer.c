#include "framebuffer.h"

#include "../zybo_video.h"

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;

static uint8_t s_fb_indexed[FB_PIXELS];
static uint32_t s_addr;

/* Matches the semantic palette from software + FPGA path. */
static const rgb_t s_palette[16] = {
    {0x00, 0x00, 0x00}, /* 0 transparent/black */
    {0x00, 0x00, 0x00}, /* 1 black */
    {0x5C, 0x94, 0xFC}, /* 2 sky */
    {0x4C, 0x78, 0xD8}, /* 3 mario blue */
    {0xFC, 0x54, 0x54}, /* 4 red */
    {0xF8, 0xB8, 0x00}, /* 5 yellow */
    {0x58, 0xD8, 0x54}, /* 6 green light */
    {0xD8, 0xA0, 0x54}, /* 7 goomba tan */
    {0xA0, 0x54, 0x2C}, /* 8 brown dark */
    {0xC8, 0x84, 0x2C}, /* 9 ground orange */
    {0xD8, 0x98, 0x5C}, /* A brick light */
    {0xA0, 0x54, 0x2C}, /* B brick dark */
    {0xFF, 0xFF, 0xFF}, /* C white */
    {0x2C, 0xA8, 0x2C}, /* D pipe green */
    {0xFC, 0xBC, 0x94}, /* E skin */
    {0x40, 0xD0, 0xFF}  /* F cyan */
};

void fb_init(void)
{
    s_addr = 0;
    if (zybo_video_init() == 0)
        fb_clear(0x02);
}

void fb_set_addr(uint32_t addr)
{
    s_addr = addr;
}

void fb_write_pixel(uint8_t color)
{
    if (s_addr < FB_PIXELS)
        s_fb_indexed[s_addr] = color & 0x0Fu;
    s_addr++;
}

void fb_write_at(uint16_t x, uint16_t y, uint8_t color)
{
    if (x >= FB_WIDTH || y >= FB_HEIGHT)
        return;
    s_fb_indexed[(uint32_t)y * FB_WIDTH + x] = color & 0x0Fu;
}

void fb_clear(uint8_t color)
{
    uint32_t i;
    uint8_t c = color & 0x0Fu;
    for (i = 0; i < FB_PIXELS; i++)
        s_fb_indexed[i] = c;
}

void fb_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color)
{
    uint16_t row, col;
    for (row = y; row < y + h && row < FB_HEIGHT; row++) {
        for (col = x; col < x + w && col < FB_WIDTH; col++)
            fb_write_at(col, row, color);
    }
}

void fb_present(void)
{
    uint8_t *frame = zybo_video_framebuffer();
    uint32_t y, x;
    const uint32_t stride = ZYBO_HDMI_STRIDE;
    const uint32_t x_offset = 64; /* center 512 active pixels inside 640 */

    if (frame == 0)
        return;

    for (y = 0; y < ZYBO_HDMI_HEIGHT; y++) {
        uint32_t row_off = y * stride;
        for (x = 0; x < ZYBO_HDMI_WIDTH; x++) {
            frame[row_off + x * 3 + 0] = 0;
            frame[row_off + x * 3 + 1] = 0;
            frame[row_off + x * 3 + 2] = 0;
        }
    }

    for (y = 0; y < FB_HEIGHT; y++) {
        uint32_t src_row = y * FB_WIDTH;
        uint32_t dy0 = y * 2;
        uint32_t dy1 = dy0 + 1;

        for (x = 0; x < FB_WIDTH; x++) {
            rgb_t p = s_palette[s_fb_indexed[src_row + x] & 0x0Fu];
            uint32_t dx0 = x_offset + x * 2;
            uint32_t dx1 = dx0 + 1;
            uint32_t o00 = dy0 * stride + dx0 * 3;
            uint32_t o01 = dy0 * stride + dx1 * 3;
            uint32_t o10 = dy1 * stride + dx0 * 3;
            uint32_t o11 = dy1 * stride + dx1 * 3;

            frame[o00 + 0] = p.b; frame[o00 + 1] = p.g; frame[o00 + 2] = p.r;
            frame[o01 + 0] = p.b; frame[o01 + 1] = p.g; frame[o01 + 2] = p.r;
            frame[o10 + 0] = p.b; frame[o10 + 1] = p.g; frame[o10 + 2] = p.r;
            frame[o11 + 0] = p.b; frame[o11 + 1] = p.g; frame[o11 + 2] = p.r;
        }
    }

    zybo_video_flush();
}
