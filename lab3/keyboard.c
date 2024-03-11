#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"

int (kb_subscribe_int)(uint8_t *bit_no){
    if(bit_no== NULL){
        return 1;
    }
    *bit_no = BIT(Hook_ID_KEYBOARD);
    if(sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE|IRQ_EXCLUSIVE, Hook_ID_KEYBOARD) != 0){
        return 1;
    }

    return 0;
}


int (timer_unsubscribe_int)(){
    if(sys_irqrmpolicy(Hook_ID_KEYBOARD) != 0){
        return 1;
    }

return 0;
} 