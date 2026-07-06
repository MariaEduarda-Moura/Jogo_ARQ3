#include "timer.h"

#include "xtime_l.h"
#include "xparameters.h"

static XTime s_start_ticks;
static uint64_t s_ticks_per_ms;

static uint64_t timer_ticks_per_second(void)
{
#if defined(XPAR_CPU_CORE_CLOCK_FREQ_HZ)
    return (uint64_t)XPAR_CPU_CORE_CLOCK_FREQ_HZ / 2U;
#elif defined(XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ)
    return (uint64_t)XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2U;
#elif defined(XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ)
    return (uint64_t)XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2U;
#else
    return 333333333ULL;
#endif
}

void timer_init(void)
{
    uint64_t ticks_per_second = timer_ticks_per_second();

    XTime_GetTime(&s_start_ticks);
    s_ticks_per_ms = ticks_per_second / 1000U;
    if (s_ticks_per_ms == 0U)
        s_ticks_per_ms = 1U;
}

uint32_t timer_millis(void)
{
    XTime now;
    uint64_t delta;

    XTime_GetTime(&now);
    delta = (uint64_t)(now - s_start_ticks);
    return (uint32_t)(delta / s_ticks_per_ms);
}

void timer_delay_ms(uint32_t ms)
{
    uint32_t start = timer_millis();
    while ((timer_millis() - start) < ms) {
    }
}
