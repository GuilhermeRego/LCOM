#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8254.h"
#include "i8042.h"
#include "keyboard.h"
#include "kbc.h"
#include "timer.h"

uint8_t scancode = 0;
uint32_t cnt = 0;
int keyboard_hook_id = 1;
extern int counter;

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
    uint8_t irq_set;
    message msg;

	if (keyboard_subscribe_int(&irq_set) != 0) return 1;	// Subscrever interrupções

	while (scancode != ESC_BREAKCODE) {		// Enquanto a tecla premida não for ESC, continua a fazer scan

		if (driver_receive(ANY, &msg, &ipc_status) != 0) return 1;

		if (is_ipc_notify(ipc_status)) {
			switch(_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.m_notify.interrupts & irq_set) {
						kbc_ih();			// Chama interrupt handler que incrementa o contador de sysinb
						uint8_t two_bytes = (scancode == TWO_BYTES) ? 2 : 1;
						bool make_code = (scancode == MAKE_CODE) ? true : false;
						if (kbd_print_scancode(make_code, two_bytes, &scancode) != 0) return 1;
					}
			}
		}
	}

	if (kbd_print_no_sysinb(cnt) != 0) return 1;
	if (keyboard_unsubscribe_int() != 0) return 1;

	return 0;
}

int(kbd_test_poll)() {

	while (scancode != ESC_BREAKCODE) {
		if (kbc_poll() != 0) return 1;  
	}

	if (write_kbc_command(WRITE_COMMAND) != 0) return 1;
	if (kbd_print_no_sysinb(cnt) != 0) return 1;

	return 0;
}

int (kbd_test_timed_scan)(uint8_t idle) {

	int ipc_status;
    uint8_t irq_set;
    message msg;

	if (keyboard_subscribe_int(&irq_set) != 0) return 1;
	while (scancode == ESC_BREAKCODE && !idle) {

		if (driver_receive(ANY, &msg, &ipc_status) != 0) return 1;

		if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)){
                 case HARDWARE:
                    if (msg.m_notify.interrupts & KEYBOARD_IRQ) {
                        kbc_ih();			
						uint8_t two_bytes = (scancode == TWO_BYTES) ? 2 : 1;
						bool make_code = (scancode == MAKE_CODE) ? true : false;
						if (kbd_print_scancode(make_code, two_bytes, &scancode) != 0) return 1;
                    }

                    if (msg.m_notify.interrupts & TIMER_IRQ) {
                        timer_int_handler();
                        if (counter % 60 == 0) idle--;;
                    }
            }
        }
	}

	if (kbd_print_no_sysinb(cnt) != 0) return 1;
	if (timer_unsubscribe_int() != 0) return 1;
	if (keyboard_unsubscribe_int() != 0) return 1;

    return 0;
}
