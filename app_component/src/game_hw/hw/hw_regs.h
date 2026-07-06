#ifndef HW_REGS_ZYBO_H
#define HW_REGS_ZYBO_H

#include <stdint.h>
#include "../platform_config.h"
#include "xil_io.h"

#define FB_BASE      0x43C00000u
#define KEY_PIO_BASE ZYBO_BTN_GPIO_BASEADDR

#define FB_REG_ADDR   0x00u
#define FB_REG_COLOR  0x04u
#define FB_REG_STATUS 0x08u

#if (ZYBO_BTN_GPIO_CHANNEL == 2U)
#define KEY_REG_DATA  0x08u
#else
#define KEY_REG_DATA  0x00u
#endif

static inline void mmio_write32(uint32_t addr, uint32_t val)
{
    Xil_Out32(addr, val);
}

static inline uint32_t mmio_read32(uint32_t addr)
{
    return Xil_In32(addr);
}

#endif /* HW_REGS_ZYBO_H */
