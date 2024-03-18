#include <lcom/lcf.h>
#include <stdint.h>

int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) {
    return 1;}
  uint32_t val;
  if(sys_inb(port, &val) != 0) {
    return 1;}
  *value = 0xFF & val;
  return 0;
}
