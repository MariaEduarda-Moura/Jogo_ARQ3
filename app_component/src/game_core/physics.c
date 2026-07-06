#include "physics.h"

static uint8_t jump_prev = 0;

void physics_reset(void)
{
    jump_prev = 0;
}

void physics_update(Mario *m, const InputState *input)
{
    uint8_t jump_now = input->jump && !jump_prev;

    jump_prev = input->jump;

    if (input->left) {
        m->vx = -WALK_SPEED;
        m->facing_left = 1;
        if (m->on_ground)
            m->state = MARIO_STATE_WALK;
    } else if (input->right) {
        m->vx = WALK_SPEED;
        m->facing_left = 0;
        if (m->on_ground)
            m->state = MARIO_STATE_WALK;
    } else {
        m->vx = 0.0f;
        if (m->on_ground)
            m->state = MARIO_STATE_IDLE;
    }

    if (jump_now && m->on_ground) {
        m->vy = JUMP_VELOCITY;
        m->on_ground = 0;
        m->state = MARIO_STATE_JUMP;
    }

    if (!m->on_ground) {
        m->vy += GRAVITY;
        if (m->vy > MAX_FALL)
            m->vy = MAX_FALL;
        m->state = (m->vy < 0.0f) ? MARIO_STATE_JUMP : MARIO_STATE_FALL;
    }

    if (m->state == MARIO_STATE_WALK)
        m->anim_frame++;
}
