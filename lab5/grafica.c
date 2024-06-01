#include <lcom/lcf.h>
#include "grafica.h"
#include <math.h>

int (set_graphic)(uint16_t submode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));
    reg86.intno = 0x10;              
    reg86.ax = 0x4F02;             
    reg86.bx = submode | BIT(14);    
    if (sys_int86(&reg86) != 0) {  
        return 1;
    }
    return 0;
}


int (set_frmbuff)(uint16_t mode){
    memset(&modeinf,0,sizeof(modeinf));
    if(vbe_get_mode_info(mode,&modeinf) != 0){
        return 1;
    }

    unsigned int bytesppi = (modeinf.BitsPerPixel + 7) /8;
    
    unsigned int framesz = modeinf.XResolution*modeinf.YResolution*bytesppi;

    struct minix_mem_range addrsfis;

    addrsfis.mr_base = modeinf.PhysBasePtr;
    addrsfis.mr_limit = modeinf.PhysBasePtr + framesz;

    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &addrsfis)){
        return 1;
    }

    frm_buff = vm_map_phys(SELF, (void*) addrsfis.mr_base, framesz);

    if(frm_buff == NULL) return 1;

    return 0;
}


int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){

    for(unsigned int d = 0; d< len; d++){

        uint16_t temp = x + d;

        if((temp)>modeinf.XResolution || y > modeinf.YResolution){
            return 1;
        }

        unsigned int bytespp = (modeinf.BitsPerPixel + 7)/8;

        unsigned int ind = (modeinf.XResolution*y + temp) * bytespp;

        if(memcpy(&frm_buff[ind],&color,bytespp) == NULL){
            return 1;
        }
        

    }
    
    return 0;

}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for(unsigned i = 0; i < height ; i++){
    if (vg_draw_hline(x, y+i, width, color) != 0) {
      vg_exit();
      return 1;
    }
    }
  return 0;

}


int normalize_color(uint32_t color, uint32_t *nclr) {
  if (modeinf.BitsPerPixel == 32) {
    *nclr = color;
  } else {
    *nclr = color & (BIT(modeinf.BitsPerPixel) - 1);
  }
  return 0;
}

uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
  return (first + (row * n + col) * step) % (1 << modeinf.BitsPerPixel);
}
uint32_t (Red)(unsigned j, uint8_t step, uint32_t first) {
  return (R(first) + j * step) % (1 << modeinf.RedMaskSize);
}

uint32_t (Green)(unsigned i, uint8_t step, uint32_t first) {
  return (G(first) + i * step) % (1 << modeinf.GreenMaskSize);
}

uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first) {
  return (B(first) + (i + j) * step) % (1 << modeinf.BlueMaskSize);
}

uint32_t (R)(uint32_t first){
  return ((1 << modeinf.RedMaskSize) - 1) & (first >> modeinf.RedFieldPosition);
}

uint32_t (G)(uint32_t first){
  return ((1 << modeinf.GreenMaskSize) - 1) & (first >> modeinf.GreenFieldPosition);
}

uint32_t (B)(uint32_t first){
  return ((1 << modeinf.BlueMaskSize) - 1) & (first >> modeinf.BlueFieldPosition);
}

uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B) {
  return (R << modeinf.RedFieldPosition) | (G << modeinf.GreenFieldPosition) | (B << modeinf.BlueFieldPosition);
}


int (show_xpm)(xpm_map_t xpm, uint16_t x,uint16_t y){
    xpm_image_t image;

    uint8_t *clr = xpm_load(xpm, XPM_INDEXED, &image);


    for(int d = 0 ; d < image.height ; d++){
        for(int b = 0 ; b < image.width ; b++){
        uint16_t temp = x + b;
        uint16_t temp2 = y + d;

        if(((temp)>modeinf.XResolution) || ((temp2)> modeinf.YResolution)){
            return 1;
        }

        unsigned int bytespp = (modeinf.BitsPerPixel + 7)/8;

        unsigned int ind = (modeinf.XResolution*(temp2) + temp)*bytespp;

        if(memcpy(&frm_buff[ind],&clr,bytespp) == NULL){
            return 1;
        }
       
         clr++;
        }
    }

    return 0;
}
