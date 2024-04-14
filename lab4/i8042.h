#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define irq_mouse 12

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

#define ACK 0xFA
#define NACK 0xFE
#define Error 0xFC

#define Enable_report 0xF4
#define Disabe_report 0xF5
#define Write_Bmouse 0xD4
#define Mouse_Rdata 0xEB
#define En_streamm 0xEA

#define mouseLB BIT(0)
#define mouseRB BIT(1)
#define mouseMB BIT(2)
#define FByte BIT(3)
#define mouse_xdelta BIT(4)
#define mouse_ydelta BIT(5)
#define mouse_xovfl BIT(6)
#define mouse_yovfl     BIT(7)

#endif
