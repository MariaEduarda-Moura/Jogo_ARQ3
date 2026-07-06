#include "sprites.h"
#include "sprites_gfx.h"

const uint8_t *sprite_for_state(uint8_t state, uint8_t anim_frame)
{
    switch (state) {
    case MARIO_STATE_WALK:
        return ((anim_frame / WALK_ANIM_STEP) & 1) ? mario_walk2 : mario_walk1;
    case MARIO_STATE_JUMP:
    case MARIO_STATE_FALL:
        return mario_jump;
    default:
        return mario_idle;
    }
}
