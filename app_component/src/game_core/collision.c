#include "collision.h"
#include "world.h"

static uint8_t rect_overlaps_solid(float x, float y, float w, float h)
{
    int32_t left = (int32_t)x;
    int32_t right = (int32_t)(x + w - 1.0f);
    int32_t top = (int32_t)y;
    int32_t bottom = (int32_t)(y + h - 1.0f);
    int32_t px, py;

    for (py = top; py <= bottom; py += (bottom > top) ? (bottom - top) : 1) {
        for (px = left; px <= right; px += (right > left) ? (right - left) : 1) {
            if (world_tile_solid(world_tile_at(px, py)))
                return 1;
        }
    }
    return 0;
}

static uint8_t feet_on_ground(float hb_x, float hb_y, float hb_w, float hb_h)
{
    int32_t left = (int32_t)hb_x;
    int32_t right = (int32_t)(hb_x + hb_w - 1.0f);
    int32_t probe_y = (int32_t)(hb_y + hb_h);
    int32_t px;

    for (px = left; px <= right; px += 4) {
        if (world_tile_solid(world_tile_at(px, probe_y)))
            return 1;
    }
    return 0;
}

void collision_resolve(Mario *m)
{
    float hb_x = m->x + 2.0f;
    float hb_y = m->y;
    float hb_w = MARIO_HITBOX_W;
    float hb_h = MARIO_HITBOX_H;
    float new_x = hb_x + m->vx;
    float new_y = hb_y + m->vy;

    m->on_ground = 0;

    if (rect_overlaps_solid(new_x, hb_y, hb_w, hb_h)) {
        if (m->vx > 0.0f)
            new_x = (float)(((int32_t)(new_x + hb_w) / TILE_SIZE) * TILE_SIZE - (int32_t)hb_w - 1);
        else if (m->vx < 0.0f)
            new_x = (float)(((int32_t)new_x / TILE_SIZE + 1) * TILE_SIZE);
        m->vx = 0.0f;
    }
    hb_x = new_x;

    if (rect_overlaps_solid(hb_x, new_y, hb_w, hb_h)) {
        if (m->vy > 0.0f) {
            new_y = (float)(((int32_t)(new_y + hb_h) / TILE_SIZE) * TILE_SIZE - (int32_t)hb_h);
            m->vy = 0.0f;
            m->on_ground = 1;
        } else if (m->vy < 0.0f) {
            new_y = (float)(((int32_t)new_y / TILE_SIZE + 1) * TILE_SIZE);
            m->vy = 0.0f;
        }
    }
    hb_y = new_y;

    if (!m->on_ground && m->vy >= 0.0f && feet_on_ground(hb_x, hb_y, hb_w, hb_h)) {
        m->on_ground = 1;
        m->vy = 0.0f;
    }

    m->x = hb_x - 2.0f;
    m->y = hb_y;

    if (m->y > (float)(LEVEL_HEIGHT * TILE_SIZE)) {
        m->x = 48.0f;
        m->y = 160.0f;
        m->vx = 0.0f;
        m->vy = 0.0f;
        m->on_ground = 0;
    }
}
