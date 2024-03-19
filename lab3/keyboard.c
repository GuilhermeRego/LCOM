#include <lcom/lcf.h>

#include "i8042.h"
#include "keyboard.h"
#include "kbc.h"

extern uint8_t scancode;
extern uint32_t cnt;
extern int keyboard_hook_id;

void (kbc_ih)() {
    if (read_kbc_output(&scancode) != 0) return 1;
    cnt++;
}

int kbc_poll() {
    if (read_kbc_output(&scancode) != 0) return 1;
    if (write_kbc_command(WRITE_COMMAND) != 0) return 1;
    cnt++;
    return 0;
}

int (keyboard_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(keyboard_hook_id);
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int (keyboard_unsubscribe_int)() {
    return sys_irqrmpolicy(&keyboard_hook_id);
}
