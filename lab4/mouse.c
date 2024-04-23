#include <lcom/lcf.h>
#include "mouse.h"


uint8_t mspck;
int mouse_hookid = 3;
uint8_t byteind = 0; 
struct packet pp;
uint8_t msbyte[3];
uint8_t scancode;



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

void(mouse_ih)(){
   uint8_t stat;
    int attempts = 10;

    while (attempts){
        if (util_sys_inb(Register_stat, &stat) != 0) return;
        if (stat & OBF_Keyboard) {
            if (util_sys_inb(out_buf, &scancode) != 0) return;
            if (byteind == 0 && (scancode & FByte)) {
                pp.bytes[byteind] = scancode;
                pp.rb = scancode & mouseRB;
                pp.mb = scancode & mouseMB;
                pp.lb = scancode & mouseLB;
                pp.delta_x = (scancode & mouse_xdelta) ? 0xFF00 : 0x0000;
                pp.delta_y = (scancode & mouse_ydelta) ? 0xFF00 : 0x0000;
                pp.x_ov = scancode & mouse_xovfl;
                pp.y_ov = scancode & mouse_yovfl;
                byteind++;
                return;
            }
            pp.bytes[byteind] = scancode;
            if (byteind == 1) {
                pp.delta_x = (pp.bytes[0] & mouse_xdelta) ? (0xFF00 | scancode) : (0x0000 | scancode);
            }
            else if (byteind == 2) {
                pp.delta_y = (pp.bytes[0] & mouse_ydelta) ? (0xFF00 | scancode) : (0x0000 | scancode);
            }
            byteind++;
            return;
        }
        attempts--;
        tickdelay(micros_to_ticks(DELAY_US));
    }

}

int (mouse_write)(uint8_t command) {

  uint8_t attemps = 10;
  uint8_t mouse_resp;

  do {
    attemps--;
    if (write_com(in_cmd, Write_Bmouse)) return 1;
    if (write_com(out_cmd, command)) return 1;
    tickdelay(micros_to_ticks(2000));
    if (util_sys_inb(out_cmd, &mouse_resp)) return 1;
    if (mouse_resp == ACK) return 0;
  } while (mouse_resp != ACK && attemps);

  return 1;
}
