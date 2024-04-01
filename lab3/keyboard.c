#include <lcom/lcf.h>

#include "i8042.h"
#include "keyboard.h"
#include "kbc.h"

extern int keyboard_hook_id;
extern int cnt;
extern uint8_t scancode;

int (kbd_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(keyboard_hook_id);
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int (kbd_unsubscribe_int)() {
    return sys_irqrmpolicy(&keyboard_hook_id);
}

void (kbc_ih)() {
    read_kbc_status(&scancode);
    cnt++;
}

int (keyboard_restore)() {
    uint8_t command;
    if (write_kbc_command(KBC_IN_BUF, KBC_READ_CMD) != 0) return 1; // Avisar que vamos ler 
    if (read_kbc_output(KBC_OUT_BUF, &command) != 0) return 1;

    command |= ENABLE_INT;

    if (write_kbc_command(KBC_IN_BUF, KBC_WRITE_CMD) != 0) return 1;
    if (write_kbc_command(KBC_WRITE_CMD, command) != 0) return 1;

    return 0;
}
