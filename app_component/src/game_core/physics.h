#ifndef PHYSICS_H
#define PHYSICS_H

#include "game.h"

#define GRAVITY        0.5f
#define JUMP_VELOCITY -8.0f
#define MAX_FALL       6.0f
#define WALK_SPEED     2.0f

void physics_reset(void);
void physics_update(Mario *m, const InputState *input);

#endif
