#include <lcom/lcf.h>

#include "menu.h"

int option = 0;
bool is_mouse_over = false;

void draw_menu() {
    if ((cursor->x > 265 && cursor->x < 500) && (cursor->y > 215 && cursor->y < 273)) {
        option = 0;
        is_mouse_over = true;
    }
    
    else if ((cursor->x > 265 && cursor->x < 515) && (cursor->y > 273 && cursor->y < 327)) {
        option = 1;
        is_mouse_over = true;
    }
    
    else if ((cursor->x > 265 && cursor->x < 515) && (cursor->y > 327 && cursor->y < 385)) {
        option = 2;
        is_mouse_over = true;
    }
    
    else if ((cursor->x > 265 && cursor->x < 500) && (cursor->y > 385 && cursor->y < 435)) {
        option = 3;
        is_mouse_over = true;
    }
    else {
        is_mouse_over = false;
    }

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

