#ifndef KBC_H_
#define KBC_H_


#include <lcom/lcf.h>
#include "i8042.h"

#define DELAY_US    20000

int read_kbc_stat(uint8_t* stat);

int read_out(uint8_t reg, uint8_t* out);

#endif
