#include <lcom/lcf.h>
#include "i8042.h"

int (write_kbc_command)(uint8_t port, uint8_t cmd) {
    int attempts = 10;
    uint8_t stat;
    while(attempts) {
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
