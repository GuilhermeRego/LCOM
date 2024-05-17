#include <lcom/lcf.h>

#include "menu.h"

int option = 0;

extern Sprite *menu;
extern Sprite *test;

extern vbe_mode_info_t mode_info;
extern uint8_t *first_buffer;

void draw_menu() {
    draw_sprite(menu, 0, 0);
}

