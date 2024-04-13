#include <lcom/lcf.h>
#include "i8042.h"

extern int attempts;

int (read_kbc_status)(uint8_t *status) {
    return util_sys_inb(KBC_OUT_BUF, status);
}

int (write_kbc_command)(uint8_t port, uint8_t cmd) {
    uint8_t stat;
    while(!attempts) {
        if (util_sys_inb(KBC_ST_REG, &stat) != 0) return 1; 
        if(!(stat & KBC_IBF)) {
            if (sys_outb(port, cmd) != 0) return 1; 
            return 0;
        }
        attempts--;
        tickdelay(micros_to_ticks(DELAY_US)); 
    }
    return 0;
}

int (read_kbc_output)(uint8_t port, uint8_t *data, bool mouse) {
    uint8_t stat;
    while(attempts) {
        if (read_kbc_status(&stat) != 0) return 1; 
        if((stat & KBC_OBF) && (stat & KBC_AUX) && mouse) {
            if (util_sys_inb(port, data) != 0) return 1; 
            if (stat & KBC_PAR_TIMEOUT) return 1;
            return 0;
        }
        attempts--;
        tickdelay(micros_to_ticks(DELAY_US)); // e.g. tickdelay()
    }
    return 1;
}
