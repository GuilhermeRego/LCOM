#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

#define TIMER_FREQ 1193182


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < 19) return 1;
  printf("%s is not yet implemented!\n", __func__);

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */
  if(st == NULL) return 1;
  if(timer > 2) return 1;
  uint8_t rb_cmd = BIT(7)|BIT(6)|BIT(4)|BIT(timer + 1);
  int outf = sys_outb(0x43, rb_cmd);
  if(outf != 0) return 1;
  int inf = util_sys_inb(timer + TIMER_0, st);
  if(inf != 0) return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,enum timer_status_field field) {
  /* To be implemented by the students */
  return 1;
}
