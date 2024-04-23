// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "i8042.h"
#include "i8254.h"
#include "KBC.c"
#include "mouse.h"
#include "timer.c"

extern struct packet pp;
extern uint8_t byteind;
extern int counter;
extern uint8_t scancode;
// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
      
  int ipc_status;
  message msg;
  uint8_t mouse_id;
  

  if (mouse_write(Enable_report) != 0) return 1; 
  if (ms_subscribe_int(&mouse_id) != 0) return 1;

  while (cnt) {

    if (driver_receive(ANY, &msg, &ipc_status) != 0){
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE: 
          if (msg.m_notify.interrupts & mouse_id){  
            mouse_ih();                               
            if (byteind == 3) {                                        
              byteind = 0;
              cnt--;
              mouse_print_packet(&pp);  
            }
          }
          break;
      }
    }
  }
  
   if (mouse_write(Disabe_report) != 0) return 1;

  if (ms_unsubscribe_int() != 0) return 1;
 
  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    int ipc_status;
  message msg;
  uint8_t sec = 0;
  uint8_t ms_id = 0, tm_id = 0;
  uint16_t timer_frequency = sys_hz();


  if (mouse_write(Enable_report) != 0) return 1;

  if (timer_subscribe_int(&tm_id) != 0) return 1;
  if (ms_subscribe_int(&ms_id) != 0) return 1;


  while (sec < idle_time) {

    if (driver_receive(ANY, &msg, &ipc_status) != 0){
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE: 

          if (msg.m_notify.interrupts & tm_id) { 
            timer_int_handler();
            if (counter % timer_frequency == 0) sec++;
          }

          if (msg.m_notify.interrupts & ms_id){  
            mouse_ih();                                                     
            if (byteind == 3) {      
              byteind = 0;
              mouse_print_packet(&pp); 
            }
            sec = 0;
            counter = 0;
          }
      }
    }
  }

  if (mouse_write(Disabe_report) != 0) return 1;


  if (timer_unsubscribe_int() != 0) return 1;
  if (ms_unsubscribe_int() != 0) return 1;

  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}


