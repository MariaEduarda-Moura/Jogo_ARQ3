#include "timer.h"

#include "sleep.h"

static uint32_t s_millis;

void timer_init(void)
{
    s_millis = 0U;
}

uint32_t timer_millis(void)
{
    return s_millis;
}

void timer_delay_ms(uint32_t ms)
{
    if (ms == 0U) {
        return;
    }
    usleep(ms * 1000U);
    s_millis += ms;
}
