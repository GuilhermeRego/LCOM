#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "i8042.h"
#include "keyboard.h"
#include "KBC.c"
#include <stdbool.h>
#include <stdint.h>

extern uint32_t kbccount;

extern uint8_t scanc;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
 
 int ipc_status;
 message msg;
 uint8_t irq_set;


 if(kb_subscribe_int(&irq_set) != 0){
  return 1;
 }

 while( scanc != Esc_break ) { 
    if ( (driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("Driver_Receive: Error");
        continue;
    }

    if (is_ipc_notify(ipc_status)) { 
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:				
                if (msg.m_notify.interrupts & irq_set) { 
                  kbc_ih();
                  if(scanc == FB_TWOBYTES){
                    kbd_print_scancode(!(scanc & Mk_code), 2, &scanc);
                  }

                  else{
                    kbd_print_scancode(!(scanc & Mk_code), 1, &scanc);
                  }
                }
        }
    } 
 }

  if(kb_unsubscribe_int() != 0){
    return 1;
  }

  if(kbd_print_no_sysinb(kbccount) != 0){
    return 1;
  }

  return 0;
}

int(kbd_test_poll)() {
  while(scanc != Esc_break){
    if(read_out(out_cmd, &scanc)!= 0){
      if(scanc == FB_TWOBYTES){
        kbd_print_scancode(!(scanc & Mk_code), 2, &scanc);
      }

      else{
        kbd_print_scancode(!(scanc & Mk_code), 1, &scanc);
      }
    }

    return 0;
  }

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
