#include "enemy.h"
#include "enemy_spawns.h"
#include "goomba_gfx.h"
#include "gfx.h"
#include "world.h"
#include "collision.h"
#include "game.h"

static Goomba goombas[MAX_GOOMBAS];

static uint8_t rects_overlap(float ax, float ay, float aw, float ah,
                             float bx, float by, float bw, float bh)
{
    return (ax < bx + bw && ax + aw > bx &&
            ay < by + bh && ay + ah > by);
}

static uint8_t goomba_feet_on_ground(const Goomba *g)
{
    int32_t left = (int32_t)g->x;
    int32_t right = (int32_t)(g->x + GOOMBA_W - 1);
    int32_t probe_y = (int32_t)(g->y + GOOMBA_H);
    int32_t px;

    for (px = left; px <= right; px += 4) {
        if (world_tile_solid(world_tile_at(px, probe_y)))
            return 1;
    }
    return 0;
}

static uint8_t goomba_hits_solid(float x, float y)
{
    int32_t left = (int32_t)x;
    int32_t right = (int32_t)(x + GOOMBA_W - 1);
    int32_t top = (int32_t)y;
    int32_t bottom = (int32_t)(y + GOOMBA_H - 1);
    int32_t px, py;

    for (py = top; py <= bottom; py += 8) {
        for (px = left; px <= right; px += 6) {
            if (world_tile_solid(world_tile_at(px, py)))
                return 1;
        }
    }
    return 0;
}

static float enemy_ground_y_at(int16_t spawn_x)
{
    int32_t ty;
    int32_t probe_x = spawn_x + (GOOMBA_W / 2);

    for (ty = LEVEL_HEIGHT - 1; ty >= 0; ty--) {
        int32_t tile_top = ty * TILE_SIZE;
        if (!world_tile_solid(world_tile_at(probe_x, tile_top)))
            continue;
        if (ty == 0 || !world_tile_solid(world_tile_at(probe_x, tile_top - 1)))
            return (float)(tile_top - GOOMBA_H);
    }
    return (float)(13 * TILE_SIZE - GOOMBA_H);
}

void enemy_reset(void)
{
    uint8_t i;

    for (i = 0; i < MAX_GOOMBAS; i++) {
        goombas[i].state = GOOMBA_INACTIVE;
        goombas[i].squish_timer = 0;
    }

    for (i = 0; i < GOOMBA_SPAWN_COUNT && i < MAX_GOOMBAS; i++) {
        goombas[i].state = GOOMBA_WALK;
        goombas[i].x = (float)goomba_spawns[i].x;
        goombas[i].y = enemy_ground_y_at(goomba_spawns[i].x);
        goombas[i].vx = -GOOMBA_SPEED;
        goombas[i].anim_frame = 0;
        goombas[i].squish_timer = 0;
    }
}

void enemy_update(void)
{
    uint8_t i;

    for (i = 0; i < MAX_GOOMBAS; i++) {
        Goomba *g = &goombas[i];

        if (g->state == GOOMBA_INACTIVE)
            continue;

        if (g->state == GOOMBA_SQUISHED) {
            if (g->squish_timer > 0)
                g->squish_timer--;
            else
                g->state = GOOMBA_INACTIVE;
            continue;
        }

        g->anim_frame++;
        g->x += g->vx;

        if (goomba_hits_solid(g->x, g->y)) {
            if (g->vx > 0.0f)
                g->x = (float)(((int32_t)(g->x + GOOMBA_W) / TILE_SIZE) * TILE_SIZE - GOOMBA_W - 1);
            else
                g->x = (float)(((int32_t)g->x / TILE_SIZE + 1) * TILE_SIZE);
            g->vx = -g->vx;
        }

        if (!goomba_feet_on_ground(g))
            g->vx = -g->vx;
    }
}

uint8_t enemy_check_mario(Mario *m)
{
    float mhx = m->x + 2.0f;
    float mhy = m->y;
    uint8_t i;

    for (i = 0; i < MAX_GOOMBAS; i++) {
        Goomba *g = &goombas[i];

        if (g->state != GOOMBA_WALK)
            continue;

        if (!rects_overlap(mhx, mhy, MARIO_HITBOX_W, MARIO_HITBOX_H,
                           g->x, g->y, GOOMBA_W, GOOMBA_H))
            continue;

        if (m->vy > 0.0f && (mhy + MARIO_HITBOX_H) < (g->y + 8.0f)) {
            g->state = GOOMBA_SQUISHED;
            g->squish_timer = 30;
            g->vx = 0.0f;
            m->vy = -4.0f;
            m->on_ground = 0;
            m->state = MARIO_STATE_JUMP;
            return 0;
        }

        return 1;
    }
    return 0;
}

void enemy_render(void)
{
    uint8_t i;

    for (i = 0; i < MAX_GOOMBAS; i++) {
        const Goomba *g = &goombas[i];
        const uint8_t *sprite;

        if (g->state == GOOMBA_INACTIVE)
            continue;

        if (g->state == GOOMBA_SQUISHED)
            sprite = goomba_squished;
        else
            sprite = (g->anim_frame & 8) ? goomba_walk2 : goomba_walk1;

        gfx_draw_indexed_at_feet(
            (int16_t)(g->x - camera.x),
            (int16_t)(g->y + GOOMBA_H),
            sprite, g->vx < 0.0f);
    }
}
