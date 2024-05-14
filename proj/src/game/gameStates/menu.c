#include <lcom/lcf.h>

#include "menu.h"

extern Sprite *menu_title;
extern vbe_mode_info_t mode_info;
extern uint8_t *video_mem;

void draw_menu() {
    memset(video_mem, 0, mode_info.YResolution * mode_info.XResolution *(( mode_info.BitsPerPixel+7) / 8));
    draw_sprite(menu_title, 400, 300);
}
