#include <lcom/lcf.h>

#include "menu.h"

int option = 0;

extern Sprite *menu_start_selected;
extern Sprite *menu_settings_selected;
extern Sprite *menu_instructions_selected;
extern Sprite *menu_exit_selected;

void draw_menu() {
    switch (option) {
        case 0:
            draw_sprite(menu_start_selected, 0, 0);
            break;
        case 1:
            draw_sprite(menu_settings_selected, 0, 0);
            break;
        case 2:
            draw_sprite(menu_instructions_selected, 0, 0);
            break;
        case 3:
            draw_sprite(menu_exit_selected, 0, 0);
            break;
    }
}

