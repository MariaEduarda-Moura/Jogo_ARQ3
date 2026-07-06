#ifndef ZYBO_VIDEO_H
#define ZYBO_VIDEO_H

#include <stdint.h>

int zybo_video_init(void);
void zybo_video_fill_rgb(uint8_t r, uint8_t g, uint8_t b);
uint8_t *zybo_video_framebuffer(void);
void zybo_video_flush(void);

#endif /* ZYBO_VIDEO_H */
