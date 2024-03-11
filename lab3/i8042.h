#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define STAT_REG 0x64
#define BUF 0x60

#define PARITY_ERROR BIT(7)
#define TIMEOUT_ERROR BIT(6)
#define OBF BIT(0)
#define IBF BIT(1)

#define KEYBOARD_IRQ 1
#define keyboard_hook_id 1

#endif