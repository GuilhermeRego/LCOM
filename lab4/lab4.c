// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "i8042.h"
#include "mouse.h"
#include "kbc.h"

int mouse_hook_id = 3;
uint8_t mouse_packet[3];
uint8_t scancode;
int byte_index = 0;
struct packet pp;
extern int counter;

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
  uint8_t irq_set;
  message msg;

  //if (mouse_write(SET_STREAM_MODE) != 0) {printf("Error set stream mode\n");return 1;}
	if (mouse_subscribe_int(&irq_set) != 0) {printf("Error subscribe_int\n");return 1;}
	if (mouse_write(ENABLE_DATA_REPORT) != 0) {printf("Error enable_data_report\n");return 1;}
  
  while (cnt) {

		if (driver_receive(ANY, &msg, &ipc_status) != 0) {printf("Error driver\n");return 1;}

		if (is_ipc_notify(ipc_status)) {
			switch(_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.m_notify.interrupts & irq_set) {
            mouse_ih();
            if (byte_index == 3) {
              byte_index = 0;
              cnt--;
              mouse_print_packet(&pp);
            }
					}
          break;
			}
		}
	}

  if (mouse_disable_data_reporting() != 0) {printf("Error disable_data_report\n");return 1;}
	if (mouse_unsubscribe_int() != 0) {printf("Error unsubscribe interrupt\n");return 1;}

	return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  int ipc_status;
  uint8_t timer_irq_set, mouse_irq_set;
  message msg;
  int seconds = 0;

  if (timer_subscribe_int(&timer_irq_set) != 0) return 1;
	if (mouse_subscribe_int(&mouse_irq_set) != 0) {printf("Error subscribe_int\n");return 1;}
	if (mouse_write(ENABLE_DATA_REPORT) != 0) {printf("Error enable_data_report\n");return 1;}

  while (1) {

		if (driver_receive(ANY, &msg, &ipc_status) != 0) {printf("Error driver\n");return 1;}

		if (is_ipc_notify(ipc_status)) {
			switch(_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
          if (msg.m_notify.interrupts & mouse_irq_set) {
            //TODO
          }
          if (msg.m_notify.interrupt & timer_irq_set) {
            timer_int_handler();
						if (counter%60 == 0) {
							timer_print_elapsed_time();
							seconds++;;
						}
          }
          break;
      }
    }
  }

  if (timer_unsubscribe_int() != 0) return 1;
  if (mouse_disable_data_reporting() != 0) {printf("Error disable_data_report\n");return 1;}
	if (mouse_unsubscribe_int() != 0) {printf("Error unsubscribe interrupt\n");return 1;}
  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
