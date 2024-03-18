#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define STAT_REG 0x64
#define BUF 0x60

#define WRITE_COMMAND 0x60

#define PARITY_TIMEOUT_ERROR BIT(7) | BIT(6)
#define OBF BIT(0)
#define IBF BIT(1)
#define ESC_BREAKCODE 0x01
#define TWO_BYTES 0xE0
#define MAKE_CODE 0xEF

#endif
