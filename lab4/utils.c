#include <lcom/lcf.h>
#include <stdint.h>

int (util_sys_inb)(int port, uint8_t *value) {  // Para prevenir erros
  if (value == NULL) return 1;                  // Verifica se é válido
  uint32_t val;                                 // sys_inb só aceita com 32 bits
  if (sys_inb(port, &val) != 0) return 1;       // meter em val o valor de port
  *value = 0xFF & val;                          // fica apenas com os 8 primeiros bits de val
  return 0;
}
