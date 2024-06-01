#include "lcom/lcf.h"

int settings_option = 0;
int resolution_option = 0;

void draw_settings() {
    switch (settings_option) {
        case 0:
            switch (resolution_option) {
                case 0:
                    draw_sprite(settings_resolution_480_selected, 0, 0);
                    break;
                case 1:
                    draw_sprite(settings_resolution_600_selected, 0, 0);
                    break;
                case 2:
                    draw_sprite(settings_resolution_768_selected, 0, 0);
                    break;
                case 3:
                    draw_sprite(settings_resolution_864_selected, 0, 0);
                    break;
                case 4:
                    draw_sprite(settings_resolution_1024_selected, 0, 0);
                    break;
            }
            break;
        case 1:
            draw_sprite(settings_clear_selected, 0, 0);
            break;
        case 2:
            draw_sprite(settings_back_selected, 0, 0);
            break;
        default:
            break;
    }
}
