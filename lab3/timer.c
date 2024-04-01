#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "timer.h"

#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "timer.h"

int timer_hook_id = 0;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq < 19 || freq > TIMER_FREQ || timer < 0 || timer > 2) return 1;
  // 1 - Construção da control word
  uint8_t controlWord;
  if (timer_get_conf(timer, &controlWord) != 0) return 1; // Obter a config do timer
  controlWord = (controlWord & 0x0F) | TIMER_LSB_MSB; // Não alterar os 4 LSB e ativar LSB followed by MSB
  uint8_t selectedTimer;  // Saber em que timer meter a nova freq
  switch (timer) {
    case 0:
      selectedTimer = TIMER_0;
      controlWord |= TIMER_SEL0;
      break;
    case 1:
      selectedTimer = TIMER_1;
      controlWord |= TIMER_SEL1;
      break;
    case 2:
      selectedTimer = TIMER_2;
      controlWord |= TIMER_SEL2;
      break;
    default:
      return 1;
  }
  if (sys_outb(TIMER_CTRL, controlWord) != 0) return 1; // Meter control word no timer control (avisar)

  uint32_t newFreq = TIMER_FREQ / freq; // Nova freq para injetar no timer selecionado
  uint8_t lsb, msb;  // Temos de meter pela ordem LSB followed by MSB
  if (util_get_LSB(newFreq, &lsb) != 0) return 1;
  if (util_get_MSB(newFreq, &msb) != 0) return 1;
  if (sys_outb(selectedTimer, lsb) != 0) return 1;  // Meter o valor da nova frequencia
  if (sys_outb(selectedTimer, msb) != 0) return 1;  // no timer selecionado

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
  if (st == NULL || timer > 2 || timer < 0) return 1;
  uint8_t selectedTimer;
  switch (timer) {
    case 0:
      selectedTimer = TIMER_0;
      break;
    case 1:
      selectedTimer = TIMER_1;
      break;
    case 2:
      selectedTimer = TIMER_2;
      break;
    default:
      return 1;
  }
  if (sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer)) != 0) return 1;  // Meter rb command no control registster
  if (util_sys_inb(selectedTimer, st) != 0) return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val data;
  switch (field) {
    case tsf_all:{
      data.byte = st;
      break;
    }
    case tsf_initial: {
      uint8_t counterInit = (st & TIMER_LSB_MSB) >> 4;
      switch (counterInit) {
        case 1:
          data.in_mode = LSB_only;
          break;
        case 2:
          data.in_mode = MSB_only;
          break;
        case 3:
          data.in_mode = MSB_after_LSB;
          break;
        default:
          data.in_mode = INVAL_val;
          return 1;
      }
      break;
    }
    case tsf_mode:
      data.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
      break;
    case tsf_base:
      data.bcd = (st & TIMER_BCD);
      break;
    default:
      return 1;
  }
  if (timer_print_config(timer, field, data) != 0) return 1;
  return 0;
}
