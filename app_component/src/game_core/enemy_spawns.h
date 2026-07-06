#ifndef ENEMY_SPAWNS_H
#define ENEMY_SPAWNS_H

#include "enemy.h"

static const GoombaSpawn goomba_spawns[] = {
    { 160, 0 },
    { 320, 0 },
    { 480, 0 },
    { 640, 0 },
};

#define GOOMBA_SPAWN_COUNT (sizeof(goomba_spawns) / sizeof(goomba_spawns[0]))

#endif
