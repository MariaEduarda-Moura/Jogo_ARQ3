#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define TILE_SIZE      16
#define LEVEL_WIDTH    200
#define LEVEL_HEIGHT   15
#define SCREEN_TILES_X 16

typedef enum {
    MARIO_STATE_IDLE,
    MARIO_STATE_WALK,
    MARIO_STATE_JUMP,
    MARIO_STATE_FALL
} MarioState;

typedef struct {
    float x, y;
    float vx, vy;
    uint8_t on_ground;
    uint8_t facing_left;
    MarioState state;
    uint8_t anim_frame;
} Mario;

typedef struct {
    int32_t x;
} Camera;

typedef struct {
    uint8_t left;
    uint8_t right;
    uint8_t jump;
    uint8_t reset;
} InputState;

void game_init(void);
void game_reset(void);
void game_update(InputState *input);
void game_render(void);

extern Mario mario;
extern Camera camera;

#endif
