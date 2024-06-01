#include <lcom/lcf.h>

int pause_option = 0;

void draw_pause() {
    switch (pause_option) {
        case 0:
            draw_sprite(pause_resume_selected, 0, 0);
            break;
        case 1:
            draw_sprite(pause_exit_selected, 0, 0);
            break;
    }
}
