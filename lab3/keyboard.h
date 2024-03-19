#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#define KEYBOARD_IRQ

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

int verify_errors(uint8_t value);

void (kbc_ih)();

int kbc_poll();

int (keyboard_subscribe_int)(uint8_t *bit_no);

int (keyboard_unsubscribe_int)();

#endif
