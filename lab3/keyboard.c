#include "keyboard.h"

uint8_t scancode = 0;
int keyboard_hook_id = 1;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(keyboard_hook_id);
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int (keyboard_unsubscribe_int)() {
    return sys_irqrmpolicy(&keyboard_hook_id);
}

void (kbc_ih)() {
    read_kbc_output(KBC_OUT_PORT, &scancode);
}

int (keyboard_restore)() {
    uint8_t command;

    if (write_kbc_command(KBC_IN_PORT, KBC_READ_CMD) != 0) return 1; // Avisar KBC que vamos ler        
    if (read_kbc_output(KBC_OUT_PORT, &command) != 0) return 1;  // Ler

    command |= ENABLE_INT;  

    if (write_kbc_command(KBC_IN_PORT, KBC_WRITE_CMD) != 0) return 1; // Avisar KBC que vamos escrever
    if (write_kbc_command(KBC_WRITE_CMD, command) != 0) return 1; // Escrever command byte alterado

    return 0;
}
