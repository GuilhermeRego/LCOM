#include "kbc.h"

extern int counter_KBC;

int (read_kbc_status) (uint8_t *status) {
  return util_sys_inb(KBC_STATUS_REG, status);
}

int (read_kbc_output) (uint8_t port, uint8_t *output) {
  uint8_t status;
  uint8_t attempts = 10;

  while (attempts) {
    if (read_kbc_status(&status) != 0) return 1;
    if ((status & BIT(0)) != 0) {
      if (util_sys_inb(port, output) != 0) return 1;
      if ((status & PARITY) != 0) return 1;
      if ((status & TIMEOUT) != 0) return 1;
      return 0;
    }

    tickdelay(micros_to_ticks(20000));
    attempts--;
  }
  printf("FAILED TO READ KBC OUTPUT!\n");
  return 1;
}

int (write_kbc_command) (uint8_t port, uint8_t command_byte) {
  uint8_t status;
  uint8_t attempts = 10;

  while (attempts) {
    if (read_kbc_status(&status)) return 1;

    if (status & IBF) {
      if (sys_outb(port,command_byte)) return 1;
    }

    tickdelay(micros_to_ticks(20000));
    attempts--;
  }
  return 1;
}
