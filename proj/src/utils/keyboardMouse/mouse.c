#include <lcom/lcf.h>
#include "i8042.h"
#include "kbc.c"
#include "mouse.h"
#include "../utils.c"

extern int mouse_hook_id;
extern uint8_t scancode;
extern int byte_index;
extern struct packet pp;
bool activated_buttons[3];

int (mouse_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(mouse_hook_id);
    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)() {
    return sys_irqrmpolicy(&mouse_hook_id);
}

void (mouse_ih)() {
    uint8_t stat;
    int attempts = 10;

    while (attempts){
        if (util_sys_inb(KBC_ST_REG, &stat) != 0) return;
        if (stat & KBC_OBF) {
            if (util_sys_inb(KBC_OUT_BUF, &scancode) != 0) return;
            if (byte_index == 0 && (scancode & FIRST_BYTE)) {
                pp.bytes[byte_index] = scancode;
                pp.rb = scancode & MOUSE_RB;
                pp.mb = scancode & MOUSE_MB;
                pp.lb = scancode & MOUSE_LB;
                pp.delta_x = (scancode & MSB_X_DELTA) ? 0xFF00 : 0x0000;
                pp.delta_y = (scancode & MSB_Y_DELTA) ? 0xFF00 : 0x0000;
                pp.x_ov = scancode & MOUSE_X_OVFL;
                pp.y_ov = scancode & MOUSE_Y_OVFL;
                byte_index++;
                return;
            }
            pp.bytes[byte_index] = scancode;
            if (byte_index == 1) {
                pp.delta_x = (pp.bytes[0] & MSB_X_DELTA) ? (0xFF00 | scancode) : (0x0000 | scancode);
            }
            else if (byte_index == 2) {
                pp.delta_y = (pp.bytes[0] & MSB_Y_DELTA) ? (0xFF00 | scancode) : (0x0000 | scancode);
            }
            byte_index++;
            return;
        }
        attempts--;
        tickdelay(micros_to_ticks(DELAY_US));
    }
}

int (mouse_write)(uint8_t command) {
    int attempts = 10;
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
