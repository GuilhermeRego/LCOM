#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"

uint8_t scanc = 0;
int hook_id_kb = 1;

int (kb_subscribe_int)(uint8_t *bit_no){
    if(bit_no== NULL){
        return 1;
    }
    *bit_no = BIT(hook_id_kb);
    if(sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id_kb) != 0){
        return 1;
    }

    return 0;
}


int (kb_unsubscribe_int)(){
    if(sys_irqrmpolicy(&hook_id_kb) != 0){
        return 1;
    }

return 0;
} 

void (kbc_ih)(){
    if(read_out(out_cmd,&scanc) != 0 ){
        printf("Unable to read scancode");
    }
}

int(rst)(){
    uint8_t cmdB;

    if(write_com(0x64, 0x20) != 0){
        return 1;
    }
    if(read_out(0x60, &cmdB) != 0) {
        return 1;
    }
    cmdB |= En_INT;

    if(write_com(0x64, 0x60) != 0){
        return 1;
    }
    if(write_com(0x60, cmdB) != 0){
        return 1;
    }

    return 0;
} 
