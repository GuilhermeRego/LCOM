#include "kbc.h"
#include "i8042.h"

extern int attempts;

int (read_kbc_status)(uint8_t *status) {
    return util_sys_inb(KBC_OUT_BUF, status);
}

int (write_kbc_command)(uint8_t port, uint8_t cmd) {
    uint8_t stat;
    while(!attempts) {
        if (util_sys_inb(KBC_ST_REG, &stat) != 0) return 1; /* assuming it returns OK */
        /* loop while 8042 input buffer is not empty */
        if( (stat & KBC_IBF) == 0 ) {
            if (sys_outb(port, cmd) != 0) return 1; /* no args command */
            return 0;
        }
        attempts--;
        tickdelay(micros_to_ticks(DELAY_US)); // e.g. tickdelay()
    }
    return 0;
}

int (read_kbc_output)(uint8_t port, uint8_t *data) {
    uint8_t stat;
    while(attempts) {
        if (read_kbc_status(&stat) != 0) return 1; /* assuming it returns OK */
        /* loop while 8042 output buffer is empty */
        if(stat & KBC_OBF) {
            if (util_sys_inb(port, data) != 0) return 1; /* ass. it returns OK */
            if (stat & (KBC_PAR_ERR | KBC_TO_ERR)) return 1;
            return 0;
        }
        attempts--;
        tickdelay(micros_to_ticks(DELAY_US)); // e.g. tickdelay()
    }
    return 1;
}
