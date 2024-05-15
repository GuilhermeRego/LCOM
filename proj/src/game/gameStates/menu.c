#include <lcom/lcf.h>

#include "menu.h"

int option = 0;

extern Sprite *menu;
extern Sprite *test;

extern vbe_mode_info_t mode_info;
extern uint8_t *first_buffer;

void draw_menu() {
    memset(first_buffer, 0, mode_info.YResolution * mode_info.XResolution *(( mode_info.BitsPerPixel+7) / 8)); // clear screen
    draw_sprite(menu, 0, 0);
}

