#include <lcom/lcf.h>

int game_over_option = 0;

void draw_game_over() {
    switch (game_over_option) {
        case 0:
            draw_sprite(gameover_tryagain_selected, 0, 0);
            break;
        case 1:
            draw_sprite(gameover_exit_selected, 0, 0);
            break;
    }
}
