#include "game.h"
#include "physics.h"
#include "collision.h"
#include "camera.h"
#include "render.h"
#include "enemy.h"

Mario mario;
Camera camera;

void game_reset(void)
{
    physics_reset();
    mario.x = 48.0f;
    mario.y = 160.0f;
    mario.vx = 0.0f;
    mario.vy = 0.0f;
    mario.on_ground = 0;
    mario.facing_left = 0;
    mario.state = MARIO_STATE_IDLE;
    mario.anim_frame = 0;
    camera_init(&camera);
    enemy_reset();
}

void game_init(void)
{
    render_init();
    game_reset();
}

void game_update(InputState *input)
{
    if (input->reset) {
        game_reset();
        return;
    }
    physics_update(&mario, input);
    collision_resolve(&mario);
    enemy_update();
    if (enemy_check_mario(&mario))
        game_reset();
    camera_update(&camera, mario.x);
}

void game_render(void)
{
    render_frame();
}
