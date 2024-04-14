#include <lcom/lcf.h>
#include "mouse.h"


uint8_t mspck;
int mouse_hookid = 3;

int(ms_subscribe_int)(uint8_t *bit_no){
    if(bit_no == NULL){
        return 1;
    }
    *bit_no = BIT(mouse_hookid);
    if (sys_irqsetpolicy(irq_mouse, IRQ_REENABLE|IRQ_EXCLUSIVE, &mouse_hookid) != 0){
        return 1;
    }
    return 0;
}


int(ms_unsubscribe_int)(){
    if(sys_irqrmpolicy(&mouse_hookid) != 0){
        return 1;
    }
    return 0;
}

void(ms_ih)(){
    if(read_out(out_cmd,&mspck,1) != 0){
        printf("Unable to read");
    }
}
