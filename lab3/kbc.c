#include "i8042.h"

int (read_kbc_status) (uint8_t *status) {
  return util_sys_inb(STAT_REG, status);
}

int (read_kbc_output) (uint8_t *output) {
  uint8_t status;
  uint8_t attempts = 10;

  while (attempts) {
    if (read_kbc_status(&status)) return 1;

    if (((status & OBF) | (status & PARITY_TIMEOUT_ERROR)) == 0) {
      if (util_sys_inb(BUF, output) != 0) return 1;
            return 0;
    }

    tickdelay(micros_to_ticks(20000));
    attempts--;
  }
  return 1;
}

int (read_kbc_input) (uint8_t command_byte) {
  uint8_t status;
  uint8_t attempts = 10;

  while (attempts) {
    if (read_kbc_status(&status)) return 1;

    if (((status & IBF) | (status & PARITY_TIMEOUT_ERROR)) == 0) {
      if (sys_outb(STAT_REG, WRITE_COMMAND)) return 1;
      if (sys_outb(BUF,command_byte)) return 1;
    }

    tickdelay(micros_to_ticks(20000));
    attempts--;
  }
  return 1;
}