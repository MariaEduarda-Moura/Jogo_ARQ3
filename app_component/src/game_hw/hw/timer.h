#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_init(void);
void timer_delay_ms(uint32_t ms);
uint32_t timer_millis(void);

#endif
