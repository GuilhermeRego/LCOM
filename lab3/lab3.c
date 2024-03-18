#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"


uint8_t scancode = 0;
uint32_t cnt = 0;
int keyboard_hook_id = 1;

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
  uint8_t irq_set;
  if (keyboard_subscribe_int(&irq_set) != 0) return 1;
  while (scancode != ESC_BREAKCODE) {
    kbc_ih();
    uint8_t two_bytes = 1;
    if (scancode == TWO_BYTES) two_bytes = 2;
    bool make_code = false;
    if (scancode == MAKE_CODE) make_code = true;
    if (kbd_print_scancode(make_code, two_bytes, &scancode) != 0) return 1;
    if (kbd_print_no_sysinb(cnt) != 0) return 1;
    cnt = 0;
  }
  if (sys_irqrmpolicy(&keyboard_hook_id) != 0) return 1;
  return 0;
}

int(kbd_test_poll)() {
  while (scancode != ESC_BREAKCODE) {
    if (kbc_poll() != 0) return 1;  
  }
  if (sys_outb(STAT_REG, WRITE_COMMAND) != 0) return 1;
  if (sys_outb(BUF, 0x01) != 0) return 1;
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
