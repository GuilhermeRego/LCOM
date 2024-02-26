#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

#define TIMER_FREQ 1193182


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < 19) return 1;               // Verificar se é inválido
  uint8_t controlWord;                                 
  timer_get_conf(timer, &controlWord);                        // Obter a config do timer e meter na controlWord
  controlWord = (controlWord & 0x0F) | TIMER_LSB_MSB;         // Tirar o timer atual com & para metermos o que temos´
                                                              // como input (timer) e ativar os bits 5 e 6 (?)
  
  uint32_t time = TIMER_FREQ / freq;                          // Obter o tempo inicial
  uint8_t msb, lsb;
  util_get_LSB(time, lsb);                                    // Como estamos em LSB followed by MSB temos
  util_get_MSB(time, msb);                                    // de retirá-los para depois meter no timer
  controlWord |= (timer << 6);                                // Selecionar timer dado como input
  uint8_t selectedTimer;
  if (!timer) selectedTimer = TIMER_0;                        // Extrair o timer dado como input e
  else if (timer) selectedTimer = TIMER_1;                    // meter em selectedTimer para depois
  else selectedTimer = TIMER_2;                               // configurarmos
  
  sys_outb(0x43, controlWord);                                // Avisar i8254 que vamos configurar o timer
  sys_outb(selectedTimer, LSB);                               // Como é LSB followed by MSB, temos de meter
  sys_outb(selectedTimer, MSB);                               // um de cada vez (vai ser sempre assim).
                                                              // metemos o valor que queremos no timer pedido
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
  uint8_t rb_cmd = BIT(7) | BIT(6) | BIT(5) | BIT(timer + 1)  // timer + 1 porque os bits 3,2 e 1 são os responsáveis por
                                                              // selecionar o timer que queremos. Ou seja, timer = 1 ->
                                                              // BIT(1 + 1) = BIT(2) -> selecionámos o bit 2 que é o do timer 2
  sys_outb(0x43, rb_cmd);                                     // Avisar o i8254 que vamos ler a configuração
  util_sys_inb(TIMER_0 + timer, st);                          // Lemos a configuração diretamente do registo associado ao timer
  return 0;                                                   // e metemos a config do timer em st
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
