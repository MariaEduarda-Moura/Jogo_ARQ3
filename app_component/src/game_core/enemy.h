#ifndef ENEMY_H
#define ENEMY_H

#include <stdint.h>
#include "game.h"

#define MAX_GOOMBAS 8
#define GOOMBA_W    14
#define GOOMBA_H    14
#define GOOMBA_SPEED 1.0f

typedef enum {
    GOOMBA_INACTIVE,
    GOOMBA_WALK,
    GOOMBA_SQUISHED
} GoombaState;

typedef struct {
    GoombaState state;
    float x, y;
    float vx;
    uint8_t anim_frame;
    uint16_t squish_timer;
} Goomba;

typedef struct {
    int16_t x, y;
} GoombaSpawn;

void enemy_reset(void);
void enemy_update(void);
uint8_t enemy_check_mario(Mario *m);
void enemy_render(void);

#endif
