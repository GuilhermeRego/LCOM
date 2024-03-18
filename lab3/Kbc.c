#include "Kbc.h"


int read_kbc_stat(uint8_t* stat){
    if(util_sys_inb(Register_stat, stat) != 0){
        return 1;
    }

    return 0;
}



int read_out(uint8_t reg, uint8_t* out){

    uint8_t stat;
    uint8_t att = 5;
    
    while(att > 0){
        if(read_kbc_stat(&stat) != 0){
            return 1;
        }

        if((stat & OBF_Keyboard) !=0 ){
            if(util_sys_inb(reg, out) != 0){
                return 1;
            }

            if((stat & Parity_Keyboard) != 0){
                return 1;
            }

            if((stat & Timeout_Keyboard) != 0){
                return 1;
            }
            
            return 0;
        }

        tickdelay(micros_to_ticks(DELAY_US));
        att--;
    }

    return 1;
}
