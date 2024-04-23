#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "KBC.h"

int (ms_subscribe_int)(uint8_t *bit_no);

int (ms_unsubscribe_int)();

void (mouse_ih)();

int (mouse_write)(uint8_t command);

#endif
