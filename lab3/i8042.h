#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define Parity_Keyboard Bit(7)
#define Timeout_Keyboard Bit(6)
#define AUX_Keyboard Bit(5)
#define INH_Keyboard Bit(4)
#define A2_Keyboard Bit(3)
#define SYS_Keyboard Bit(2)
#define IBF_Keyboard Bit(1)
#define OBF_Keyboard Bit(0)
#define IRQ_KEYBOARD 1
#define Hook_ID_KEYBOARD 1
#define Register_stat 0x64
#define buf 0x60




#endif