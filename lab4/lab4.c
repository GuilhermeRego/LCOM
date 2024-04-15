// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "i8042.h"
#include "mouse.h"
#include "kbc.h"
#include "timer.c"

int mouse_hook_id = 3;
uint8_t scancode;
int byte_index = 0;
struct packet pp;
int counter = 0;
bool activated_buttons[3];
int n_activated_buttons = 0;


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

  if (mouse_write(ENABLE_DATA_REPORT) != 0) {printf("Error enable_data_report\n");return 1;}
	if (mouse_subscribe_int(&irq_set) != 0) {printf("Error subscribe_int\n");return 1;}
  
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
  uint16_t freq = sys_hz();

  if (mouse_write(ENABLE_DATA_REPORT) != 0) {printf("Error enable_data_report\n");return 1;}
  
  if (timer_subscribe_int(&timer_irq_set) != 0) return 1;
	if (mouse_subscribe_int(&mouse_irq_set) != 0) {printf("Error subscribe_int\n");return 1;}

  while (seconds != idle_time) {

		if (driver_receive(ANY, &msg, &ipc_status) != 0) {printf("Error driver\n");return 1;}

		if (is_ipc_notify(ipc_status)) {
			switch(_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
          if (msg.m_notify.interrupts & mouse_irq_set) {
            mouse_ih();
            if (byte_index == 3) {
              byte_index = 0;
              mouse_print_packet(&pp);
            }
            seconds = 0;
            counter = 0;
          }

          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
						if (counter%freq == 0) {
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

enum state_machine {
    START,
    FIRST_LINE,
    SECOND_LINE,
    VERTEX,
    END
};

enum state_machine sm = START;

int (update_state_machine)(uint8_t x_len, uint8_t tolerance) {
    // Check if X_DELTA and Y_DELTA are bigger than tolerance
    if (pp.bytes[0] & MSB_X_DELTA) {
        if (pp.delta_x < 0) {
            if ((pp.delta_x * -1) > tolerance) {printf("1");return 1;}
        }
        else {
            if (pp.delta_x > tolerance) {printf("2");return 1;}
        }
    }
    if (pp.bytes[0] & MSB_Y_DELTA) {
        if (pp.delta_y < 0) {
            if ((pp.delta_y * -1) > tolerance) {printf("3");return 1;}
        }
        else {
            if (pp.delta_y > tolerance) {printf("4");return 1;}
        }
    }
    if (pp.bytes[1] > x_len) {printf("5");return 1;}

    // Check if more than one button is pressed
    activated_buttons[0] = pp.lb;
    activated_buttons[1] = pp.mb;
    activated_buttons[2] = pp.rb;
    for (int i = 0; i < 3; i++) {
        if (activated_buttons[i]) n_activated_buttons++;
    }
    if (n_activated_buttons > 1) {printf("6");return 1;}

    // Handling State Machine
    switch (sm) {
        case START:
            printf("START\n");
            if (pp.lb) sm = FIRST_LINE;
            break;
        case FIRST_LINE:
            printf("FIRST_LINE\n");
            if (n_activated_buttons == 0) sm = VERTEX;
            else sm = START;
            break;
        case VERTEX:
            printf("VERTEX\n");
            if (pp.rb) sm = SECOND_LINE;
            else sm = START;
            break;
        case SECOND_LINE:
            printf("SECOND_LINE\n");
            if (n_activated_buttons == 0) sm = END;
            else sm = START;
            break;
        case END:
            return 0;
            break;
    }
    return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  int ipc_status;
  uint8_t mouse_irq_set;
  message msg;
  bool done = false;

  if (mouse_subscribe_int(&mouse_irq_set) != 0) return 1;
  if (mouse_write(ENABLE_DATA_REPORT) != 0) {printf("Error enable_data_report\n");return 1;}

  while (!done) {

		if (driver_receive(ANY, &msg, &ipc_status) != 0) {printf("Error driver\n");return 1;}

		if (is_ipc_notify(ipc_status)) {
			switch(_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
          if (msg.m_notify.interrupts & mouse_irq_set) {
            mouse_ih();
            if (update_state_machine(x_len, tolerance) == 0) done = true;
          }
      }
    }
  }

  if (mouse_disable_data_reporting() != 0) {printf("Error disable_data_report\n");return 1;}
  if (mouse_unsubscribe_int() != 0) return 1;
  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
