#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KEYBOARD_IRQ 1

#define ESC_BREAKCODE 0x81 // Breakcode da tecla ESC

#define BUF 0x60 // Buffer para leitura do teclado

#define KBC_STATUS_REG  0x64 // Status Register do controller do teclado
#define KBC_IN_PORT      0x64 // Input port do controller
#define KBC_OUT_PORT    0x60 // Output port do controller

#define KBC_READ_CMD    0x20 // Read Command a ser escrito no Input Port para "avisar da leitura"
#define KBC_WRITE_CMD   0x60 // Write Command a ser escrito no Input Port para "avisar da escrita"

// Bits de Status do teclado
#define PARITY BIT(7)
#define TIMEOUT BIT(6)
#define OBF BIT(0) 
#define IBF BIT(1)
#define TWO_BYTES 0xE0
#define MAKE_CODE 0xEF

#define ENABLE_INT BIT(0) // Bit para ativar interrupções

#endif
