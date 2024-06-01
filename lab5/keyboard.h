#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"

int (kb_subscribe_int)(uint8_t *bit_no);

int (kb_unsubscribe_int)();

void (kbc_ih)();

int (rst)();

#endif
