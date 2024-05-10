#include "kbc.h"
#include "i8042.h"
#include "utils.c"

extern int cnt;

int (read_kbc_status)(uint8_t *status) {
    if (util_sys_inb(KBC_ST_REG, status) != 0) return 1;
    cnt++;
    return 0;
}

int (write_kbc_command)(uint8_t port, uint8_t cmd) {
    uint8_t stat;
    int attempts = 10;
    while(attempts) {
        if (read_kbc_status(&stat) != 0) return 1; /* assuming it returns OK */
        /* loop while 8042 input buffer is not empty */
        if(!(stat & KBC_IBF)) {
            if (sys_outb(port, cmd) != 0) return 1; /* no args command */
            return 0;
        }
        attempts--;
        tickdelay(micros_to_ticks(DELAY_US)); // e.g. tickdelay()
    }
    return 0;
}

int (read_kbc_output)(uint8_t port, uint8_t *output) {
    uint8_t stat;
    int attempts = 10;
    while(attempts) {
        if (read_kbc_status(&stat) != 0) return 1; /* assuming it returns OK */
        /* loop while 8042 output buffer is empty */
        if((stat & KBC_OBF) != 0) {
            if (util_sys_inb(port, output) != 0) return 1; /* ass. it returns OK */
            cnt++;
            if (stat & (KBC_PAR_ERR | KBC_TO_ERR)) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US)); // e.g. tickdelay()
        attempts--;
    }
    return 1;
}
