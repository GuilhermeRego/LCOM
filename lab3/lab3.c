#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8254.h"
#include "i8042.h"
#include "keyboard.h"
#include "kbc.h"
#include "timer.h"

extern int timer_hook_id;
int keyboard_hook_id = 1;
int cnt = 0;
uint8_t scancode;
int attempts = 10;
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

int (keyboard_restore)() {
    uint8_t command;
    if (write_kbc_command(KBC_IN_BUF, KBC_READ_CMD) != 0) return 1; // Avisar que vamos ler 
    if (read_kbc_output(KBC_OUT_BUF, &command) != 0) return 1;

    command |= ENABLE_INT;

    if (write_kbc_command(KBC_IN_BUF, KBC_WRITE_CMD) != 0) return 1;
    if (write_kbc_command(KBC_WRITE_CMD, command) != 0) return 1;

    return 0;
}

int (kbd_test_scan)() {
	int ipc_status;
    uint8_t irq_set;
    message msg;

	if (keyboard_subscribe_int(&irq_set) != 0) return 1;

	while (scancode != ESC_BREAKCODE) {		// Enquanto a tecla premida não for ESC, continua a fazer scan

		if (driver_receive(ANY, &msg, &ipc_status) != 0) return 1;

		if (is_ipc_notify(ipc_status)) {	// Verifica se recebemos um interrupt
			switch(_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.m_notify.interrupts & irq_set) {
						kbc_ih();			// Chama interrupt handler que incrementa o contador de sysinb
						bool make = ((scancode & BIT(7)) > 0) ? false : true;
						uint8_t size = (scancode == TWO_BYTES) ? 2 : 1;
						if (kbd_print_scancode(make, size, &scancode) != 0) return 1;
					}
			}
		}
	}

	if (keyboard_unsubscribe_int() != 0) return 1;
	if (kbd_print_no_sysinb(cnt) != 0) return 1;

	return 0;
}

int (kbd_test_poll)() {
	while (scancode != ESC_BREAKCODE) {
		if (read_kbc_output(KBC_OUT_BUF, &scancode) == 0) {
			bool make = ((scancode & BIT(7)) > 0) ? false : true;
			uint8_t size = (scancode == TWO_BYTES) ? 2 : 1;
			kbd_print_scancode(make, size, &scancode);
		}
	}
	return keyboard_restore();
}

// Não passa em 0, 3, 4 e 5

int (kbd_test_timed_scan)(uint8_t idle) {
	int ipc_status;
    uint8_t keyboard_irq_set, timer_irq_set;
    message msg;
	int seconds = 0;

	if (keyboard_subscribe_int(&keyboard_irq_set) != 0) return 1;
	if (timer_subscribe_int(&timer_irq_set) != 0) return 1;

	while (scancode != ESC_BREAKCODE && seconds < idle) {
		if(driver_receive(ANY, &msg, &ipc_status) != 0) return 1;

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.m_notify.interrupts & keyboard_hook_id) { // Timer0 int?
						kbc_ih();
						bool make = ((scancode & BIT(7)) > 0) ? false : true;
						uint8_t size = (scancode == TWO_BYTES) ? 2 : 1;
						kbd_print_scancode(make, size, &scancode);
						seconds = 0;
						counter = 0;
					}
					if (msg.m_notify.interrupts & timer_hook_id) { // KBD int?
						timer_int_handler();
						if (counter%60 == 0) {
							timer_print_elapsed_time();
							seconds++;;
						}
					}
					break;
				default:
					break; /* no other notifications expected: do nothing */
			}
		}
	}

	if (keyboard_unsubscribe_int() != 0) return 1;
	if (timer_unsubscribe_int() != 0) return 1;
	return kbd_print_no_sysinb(cnt);
}
