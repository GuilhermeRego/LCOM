#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KEYBOARD_IRQ 1

#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_IN_BUF 0x60
#define KBC_OUT_BUF 0x60

#define KBC_READ_CMD 0x20
#define KBC_WRITE_CMD 0x60

#define KBC_IBF BIT(1)
#define KBC_OBF BIT(0)
#define DELAY_US 20000

#define KBC_PAR_ERR BIT(7)
#define KBC_TO_ERR BIT(6)

#define TWO_BYTES 0xE0

#define ESC_BREAKCODE 0x81

#define ENABLE_INT BIT(0)

#endif
