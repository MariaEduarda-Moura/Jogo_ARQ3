#include "keys.h"
#include "xgpio.h"
#include "xstatus.h"

static XGpio s_btn_gpio;
static uint8_t s_gpio_ready;

static void keys_init_once(void)
{
    int status;

    if (s_gpio_ready) {
        return;
    }

    status = XGpio_Initialize(&s_btn_gpio, ZYBO_BTN_GPIO_BASEADDR);
    if (status == XST_SUCCESS) {
        XGpio_SetDataDirection(&s_btn_gpio, ZYBO_BTN_GPIO_CHANNEL, 0xFFFFFFFFu);
        s_gpio_ready = 1u;
    }
}

/*
 * BTN mapping (LSB..MSB):
 * bit0 -> Left
 * bit1 -> Right
 * bit2 -> Jump
 * bit3 -> Reset
 */
uint8_t keys_read_raw(void)
{
    keys_init_once();
    if (s_gpio_ready) {
        return (uint8_t)(XGpio_DiscreteRead(&s_btn_gpio, ZYBO_BTN_GPIO_CHANNEL) & 0x0Fu);
    }
    return (uint8_t)(mmio_read32(KEY_PIO_BASE + KEY_REG_DATA) & 0x0Fu);
}

uint8_t keys_read_pressed(void)
{
    /* Zybo buttons are active-high in AXI GPIO readout. */
    return keys_read_raw();
}
