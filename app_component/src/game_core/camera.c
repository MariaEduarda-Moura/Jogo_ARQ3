#include "camera.h"
#include "hw/framebuffer.h"

void camera_init(Camera *cam)
{
    cam->x = 0;
}

void camera_update(Camera *cam, float mario_x)
{
    int32_t target = (int32_t)mario_x - CAM_CENTER_X + 8;
    int32_t max_scroll = LEVEL_WIDTH * TILE_SIZE - FB_WIDTH;

    if (max_scroll < 0)
        max_scroll = 0;

    if (target < 0)
        target = 0;
    if (target > max_scroll)
        target = max_scroll;

    cam->x = target;
}
