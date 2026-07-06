#include "input.h"
#include "hw/keys.h"

void input_poll(InputState *input)
{
    uint8_t keys = keys_read_pressed();
    input->left  = (keys & KEY_LEFT)  ? 1 : 0;
    input->right = (keys & KEY_RIGHT) ? 1 : 0;
    input->jump  = (keys & KEY_JUMP)  ? 1 : 0;
    input->reset = (keys & KEY_RESET) ? 1 : 0;
}
