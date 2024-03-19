#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>

int (read_kbc_status)(uint8_t* status);

int (read_KBC_output)(uint8_t port, uint8_t *output);

int (write_KBC_command)(uint8_t port, uint8_t command_byte);

int verify_errors(uint8_t value);

#endif
