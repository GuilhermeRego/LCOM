#include <lcom/lcf.h>
#include "mouse.h"


uint8_t mspck;
int mouse_hookid = 3;
uint8_t byteind = 0; 
struct packet mouse_packet;
uint8_t msbyte[3];



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

void(ms_sync_bytes)(){
    if (byteind == 0 && (mspck & FByte)) { 
    msbyte[byteind]= mspck;
    byteind++;
  }
  else if (byteind > 0) {                           
    msbyte[byteind] = mspck;
    byteind++;
  }
}

void(ms_bytes_to_pck)(){
    int d = 0;
    while(d<3){
        mouse_packet.bytes[d] = msbyte[d];
        d++;
    }

    if (msbyte[0] & mouse_xdelta) {
        mouse_packet.delta_x = 0xFF00 | msbyte[1];
    } else {
        mouse_packet.delta_x = msbyte[1];
    }

    if (msbyte[0] & mouse_ydelta) {
        mouse_packet.delta_y = 0xFF00 | msbyte[2];
    } else {
        mouse_packet.delta_y = msbyte[2];
    }
    mouse_packet.mb = msbyte[0] & mouseLB;
    mouse_packet.lb = msbyte[0] & mouseLB;
    mouse_packet.rb = msbyte[0] & mouseLB;
    mouse_packet.y_ov = msbyte[0] & mouse_yovfl;
    mouse_packet.x_ov = msbyte[0] & mouse_xovfl;

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
