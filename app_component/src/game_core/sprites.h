#ifndef SPRITES_H
#define SPRITES_H

#include <stdint.h>
#include "game.h"

/* Frames per walk pose before switching (higher = slower animation) */
#define WALK_ANIM_STEP 8

const uint8_t *sprite_for_state(uint8_t state, uint8_t anim_frame);

#endif
