#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

#define TIMER_FREQ 1193182

int counter = 0;

int timer_hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < 19) return 1;
  

  uint8_t ctrlw;
  if(timer_get_conf(timer, &ctrlw) != 0) return 1;


  ctrlw &= 0xCF;
  ctrlw |= TIMER_LSB_MSB;                               

  uint32_t init = TIMER_FREQ/freq;
  uint8_t select;
  if(timer == 0) {
    ctrlw |= TIMER_SEL0;
    select = TIMER_0;}

  else if(timer == 1) {
    ctrlw |= TIMER_SEL1;
    select = TIMER_1;}
  else if(timer == 2) {
    ctrlw |= TIMER_SEL2;
    select = TIMER_2;}
  else return 1;

  uint8_t lsb;
  uint8_t msb;
  if(util_get_LSB(init, &lsb) != 0) return 1;
  if(util_get_MSB(init, &msb) != 0) return 1;


  if(sys_outb(TIMER_CTRL, ctrlw) !=0 ) return 1;

  if(sys_outb(select, lsb) != 0) return 1;
  if(sys_outb(select, msb) != 0) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if(bit_no == NULL){
    return 1;
  }
  *bit_no = BIT(timer_hook_id);
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0){
    return 1;
  }
  return 0;
}


int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&timer_hook_id) != 0){ 
    return 1;
    }
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */
  if(st == NULL) {
    return 1;
  }
  if(timer > 2) {
    return 1;
  }
  uint8_t rb_cmd = BIT(7)|BIT(6)|BIT(5)|BIT(timer + 1);
  int outf = sys_outb(0x43, rb_cmd);
  if(outf != 0) {
    return 1;
  }
  int inf = util_sys_inb(timer + TIMER_0, st);
  if(inf != 0) {
    return 1;
  }
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,enum timer_status_field field) {
  /* To be implemented by the students */
  union timer_status_field_val confi;
  if(field == tsf_all){
    confi.byte = st;
    return 0;}

  if(field == tsf_initial){
    st = (st >> 4);
    st = st & 0x03;
    if(st == 1){
      confi.in_mode = LSB_only;
    }
    else if(st == 2){
      confi.in_mode = MSB_only;
    }
    else if(st==3){
      confi.in_mode= MSB_after_LSB;
    }
    else {
      confi.in_mode = INVAL_val;
    }
    return 0;
  }

  else if(field == tsf_mode){
    st = (st >> 1);
    st = st & 0x07;
    if(st >= 0 && st<=5){
      confi.count_mode = st;
    }
    return 0;
  }

  else if(field == tsf_base){
    confi.bcd = st & 0x01;
    return 0;
  }



  int val = timer_print_config(timer,field,confi);
  if(val != 0) {
    return 1;
  }
  return 0;
}
