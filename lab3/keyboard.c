#include <lcom/lcf.h>

#include "i8042.h"
#include "keyboard.h"
#include "kbc.h"

extern int keyboard_hook_id;
extern int cnt;
extern uint8_t scancode;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(keyboard_hook_id);
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int (keyboard_unsubscribe_int)() {
    return sys_irqrmpolicy(&keyboard_hook_id);
}

void (kbc_ih)() {
    read_kbc_status(&scancode);
    cnt++;
}
