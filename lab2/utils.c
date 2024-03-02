#include <lcom/lcf.h>
#include <stdint.h>
#define LSB_MASK 0x00FF

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;
  *lsb = val & LSB_MASK;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = val >> 8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {  // Para prevenir erros
  if (value == NULL) return 1;                  // Verifica se é válido
  uint32_t val;                                 // sys_inb só aceita com 32 bits
  sys_inb(port, &val);                          // meter em val o valor de port
  *value = 0xFF & val;                          // fica apenas com os 8 primeiros bits de val
  return 0;
}
