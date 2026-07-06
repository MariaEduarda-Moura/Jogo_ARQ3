#ifndef KEYS_H
#define KEYS_H

#include <stdint.h>
#include "hw_regs.h"

#define KEY_LEFT   0x01u
#define KEY_RIGHT  0x02u
#define KEY_JUMP   0x04u
#define KEY_RESET  0x08u

uint8_t keys_read_raw(void);
uint8_t keys_read_pressed(void);

#endif
