#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
int timer_hook_id = 0;
int counter;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < 19) return 1;
  
  uint8_t CTRL_WRD;
  if (timer_get_conf(timer,&CTRL_WRD)) return 1;

  CTRL_WRD |= TIMER_LSB_MSB | (timer << 6);

  uint32_t TIME;
  TIME = TIMER_FREQ / freq;
  uint8_t TIME_LSB; uint8_t TIME_MSB;
  if (util_get_LSB(TIME,&TIME_LSB)) return 1;
  if (util_get_MSB(TIME,&TIME_MSB)) return 1;

  if (sys_outb(TIMER_CTRL,CTRL_WRD)) return 1;
  if (sys_outb(TIMER_0 + timer,TIME_LSB)) return 1;
  if (sys_outb(TIMER_0 + timer,TIME_MSB)) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
if (bit_no == NULL) return 1;
  *bit_no = BIT(timer_hook_id);
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);
  }

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id); 
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer >= 2 || timer == 0) return 1;

  uint8_t RB_CODE = BIT(7) | BIT(6) | BIT(5) | BIT(timer + 1);

  if(sys_outb(0x43,RB_CODE) == 1) return 1;
  int v = util_sys_inb(0x40 + timer,st);

  return v;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val data;  

  switch (field) {
    case tsf_all:
      data.byte = st;
    case tsf_initial:
      st >>= 4;
      st &= (BIT(0) | BIT(1));
      if (!st) data.in_mode = INVAL_val;
      if (st == 1) data.in_mode = LSB_only;
      if (st == 2) data.in_mode = MSB_only;
      if (st == 3) data.in_mode = MSB_after_LSB;
    case tsf_mode:
      st >>= 1;
      st &= (BIT(0) | BIT(1) | BIT(2));
      data.count_mode = st;
    case tsf_base:  
      st &= BIT(1);
      data.bcd = st;
    default:
      return 1;
  }

  if(timer_print_config(timer,field,data)) return 1;

  return 0;
}
