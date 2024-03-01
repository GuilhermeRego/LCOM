#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

#define TIMER_FREQ 1193182


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < 19) return 1;               // Verificar se é inválido

  uint8_t controlWord;                                        // Temos de consultar a configuração atual do timer
  if (timer_get_conf(timer, &controlWord) != 0) return 1;     // Extrair control word para controlWord
  
  controlWord |= TIMER_LSB_MSB;                               // Ativar os bits LSB followed by MSB (padrão)
  controlWord &= (timer << 6);                                // Ativar bits 7 e 6 com o timer do input

  uint32_t time = TIMER_FREQ / freq;                          // Calcular valor com a freq fornecida
  uint8_t selectedTimer;
  if (!timer) selectedTimer = TIMER_0;                        // Extrair o timer conforme o input dado
  else if (timer) selectedTimer = TIMER_1;                    // para ser mais fácil depois meter o time
  else if (timer == 2) selectedTimer = TIMER_2;               // no selectedTimer
  else return 1;

  if (sys_outb(TIMER_CTRL, controlWord) != 0) return 1;       // Avisar que vamos fazer o que tá na controlWord ao controlador

  uint16_t lsb;                                               // Como dissemos ao controlador que vamos fazer
  uint16_t msb;                                               // com LSB followed by MSB, temos de separá-los
  if (util_get_LSB(time, &lsb) != 0) return 1;                // para depois meter um de cada vez com sys_inb
  if (util_get_MSB(time, &msb) != 0) return 1;                               

  if (sys_inb(timer, lsb) != 0) return 1;                     // Meter no timer os lsb do time
  if (sys_inb(timer, msb) != 0) return 1;                     // Meter no timer os msb do time

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
  if (st == NULL || timer >= 2 || timer == 0) return 1;       // Verificar se é inválido

  uint8_t rb_cmd = BIT(7) | BIT(6) | BIT(5) | BIT(timer + 1)  // BIT(7) e BIT(6) -> ativar o read back
                                                              // BIT(5) -> não queremos ler o counter
                                                              // BIT(timer + 1) -> ativar o bit do timer
  sys_outb(TIMER_CTRL, rb_cmd);
  util_sys_inb(TIMER_0 + timer, st);                          // Lemos a configuração diretamente do registo associado ao timer
  return 0;                                                   // e metemos a config do timer em st
}

/* Acabar esta função: perceber como se resolve vendo os slides do souto */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val data;                            // Estrutura de dado para guardarmos

  switch (field) {
    case tsf_all:                                               // Quer a configuração -> metemos no data.byte
      data.byte = st;                                           // o status do comando read-back: st
      break;
    
    // TODO
  }

  timer_print_config(timer, field, data);
  return 1;
}
