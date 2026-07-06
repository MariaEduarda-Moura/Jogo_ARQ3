#ifndef CAMERA_H
#define CAMERA_H

#include "game.h"

#define CAM_CENTER_X  (FB_WIDTH / 2)

void camera_init(Camera *cam);
void camera_update(Camera *cam, float mario_x);

#endif
