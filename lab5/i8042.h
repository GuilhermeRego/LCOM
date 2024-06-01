#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define Parity_Keyboard BIT(7)
#define Timeout_Keyboard BIT(6)
#define AUX_Keyboard BIT(5)
#define INH_Keyboard BIT(4)
#define A2_Keyboard BIT(3)
#define SYS_Keyboard BIT(2)
#define IBF_Keyboard BIT(1)
#define OBF_Keyboard BIT(0)

#define IRQ_KEYBOARD 1

#define Register_stat 0x64
#define in_buf 0x64
#define out_buf 0x60

#define Esc_break 0x81
#define Mk_code BIT(7)

#define ReadCB_Keyboard 0x20
#define FB_TWOBYTES 0XE0
#define WriteCB_Keyboard 0x60
#define out_cmd 0x60
#define in_cmd 0x64

#define En_INT BIT(0)

#endif
