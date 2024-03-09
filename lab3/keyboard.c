#include <lcom/lcf.h>

#include "i8042.h"

uint8_t scancode = 0;

int verify_errors(uint8_t value) {
    if (!value) return 0;
    if (value >= 1) return 1;
    return 1;
}

void (kbc_ih)() {
    uint8_t status;
    if (util_sys_inb(STAT_REG, &status) != 0) return 1;                                         // Lê o estado do registo do KBC (0x64)
    if (verify_errors((status & BIT(7) & BIT(6)) >> 6) != 0) return 1;                          // Verifica por erros
    if ((status & BIT(0)) != 0) {                                                               // Vê se o OBF está cheio -> se tem algo para ler
        if (util_sys_inb(BUF, &scancode) != 0) return 1;
    }
    return 0;                                                       
}

int keyboard_subscribe_int (uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(keyboard_hook_id);
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int keyboard_unsubscribe_int() {
    return sys_irqrmpolicy(&keyboard_hook_id);
}