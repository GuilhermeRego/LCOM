#include <lcom/lcf.h>
#include "i8042.h"
#include "kbc.c"
#include "mouse.h"
#include "utils.c"

extern int mouse_hook_id;
int attempts = 10;
extern uint8_t scancode;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(mouse_hook_id);
    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)() {
    return sys_irqrmpolicy(&mouse_hook_id);
}

void (mouse_ih)() {
    if (read_kbc_output(KBC_OUT_BUF, &scancode, 1) != 0) printf("Error mouse_ih");  
}

int (mouse_write)(uint8_t command) {
    uint8_t ack_byte;
    while (attempts) {
        if (write_kbc_command(KBC_CMD_REG, MOUSE_WRITE_BYTE) != 0) return 1;
        if (write_kbc_command(KBC_IN_BUF, command) != 0) return 1;
        if (util_sys_inb(KBC_OUT_BUF, &ack_byte) != 0) return 1;
        if (ack_byte == ACK) return 0;
        attempts--;
    }
    return 1;
}

int (mouse_disable_data_reporting)() {
    return mouse_write(DISABLE_DATA_REPORT); 
}
