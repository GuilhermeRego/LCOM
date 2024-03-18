#include <lcom/lcf.h>

#include "i8042.h"
#include "keyboard.h"

int verify_errors(uint8_t value) {
    if (!value) return 0;                                                            // Bit 7 e 6 de erros está a 0 -> não há erros
    if (value >= 1) return 1;                                                        // Bit 7 ou 6 ou os dois está a 1 -> há erros
    return 1;
}

void (kbc_ih)() {
    uint8_t status;
    uint8_t attempts = 10;
    cnt++;
    while (attempts) {
        if (util_sys_inb(STAT_REG, &status) != 0) return;                              // Lê o estado do registo do KBC (0x64)
        if (verify_errors((status & PARITY_TIMEOUT_ERROR) >> 6) != 0) return;          // Procura por erros

        if ((status & OBF) != 0) {                                                     // Vê se o OBF está cheio -> se tem algo para ler
            util_sys_inb(BUF, &scancode);                             // Mete no scancode o que está no BUF para ler
            return;
        }
        tickdelay(micros_to_ticks(20000));
        attempts--;
    }                                                       
}

int kbc_poll() {
    uint8_t status;
    uint8_t attempts = 10;
    cnt++;
    while (attempts) {
        if (util_sys_inb(STAT_REG, &status) != 0) return 1;                              // Lê o estado do registo do KBC (0x64)
        if (verify_errors((status & PARITY_TIMEOUT_ERROR) >> 6) != 0) return 1;          // Procura por erros

        if ((status & OBF) != 0) {                                                     // Vê se o OBF está cheio -> se tem algo para ler
            util_sys_inb(BUF, &scancode);                             // Mete no scancode o que está no BUF para ler
            return 0;
        }
        tickdelay(micros_to_ticks(20000));
        attempts--;
    }  
    return 1;
}

int (keyboard_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(keyboard_hook_id);
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int (keyboard_unsubscribe_int)() {
    return sys_irqrmpolicy(&keyboard_hook_id);
}
