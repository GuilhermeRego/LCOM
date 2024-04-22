#include "Kbc.h"

uint32_t kbccount = 0;

int read_kbc_stat(uint8_t* stat){
    if(util_sys_inb(Register_stat, stat) != 0){
        return 1;
    }

    return 0;
}



int read_out(uint8_t reg, uint8_t* out, uint8_t ms){

    uint8_t stat;
    uint8_t att = 5;
    kbccount++;
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
            
            if (ms && !(stat & AUX_Keyboard)) {          
                return 1;
            } 
            if (!ms && (stat & AUX_Keyboard)) {                
                return 1;
            } 

            return 0;
        }

        tickdelay(micros_to_ticks(DELAY_US));
        att--;
    }

    return 1;
}


int write_com(uint8_t port, uint8_t cmdB){
    uint8_t stat;
    uint8_t att = 12;

    while(att){
        if(read_kbc_stat(&stat) != 0){
            return 1;
        }

        if((stat & IBF_Keyboard ) == 0){
            if(sys_outb(port, cmdB) != 0){
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
        att--;

    }

    return 1;
}
