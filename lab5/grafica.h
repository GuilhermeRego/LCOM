#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include "VBE.h"

vbe_mode_info_t modeinf;
uint8_t* frm_buff;


int (set_graphic)(uint16_t submode);
int (set_frmbuff)(uint16_t mode);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int normalize_color(uint32_t color, uint32_t *nclr);
uint32_t (Red)(unsigned j, uint8_t step, uint32_t first);
uint32_t (Green)(unsigned i, uint8_t step, uint32_t first);
uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first);
uint32_t (R)(uint32_t first);
uint32_t (G)(uint32_t first);
uint32_t (B)(uint32_t first);
uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B);
uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n);
int (show_xpm)(xpm_map_t xpm, uint16_t x,uint16_t y);

#endif
