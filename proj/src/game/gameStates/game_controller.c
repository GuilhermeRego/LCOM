#include <lcom/lcf.h>

#include "game_controller.h"

typedef struct {
    int x, y;
    int xspeed, yspeed;
} laser_t;

laser_t lasers[100];
int laser_index = 0;
int selected_cannon = 0;

void draw_lasers();

void draw_cannon();

void draw_game() {
    draw_sprite(background, 0, 0);
    draw_sprite(player, player->x, player->y);
    draw_cannon();
    draw_lasers();
}

void create_laser() {
    switch (selected_cannon) {
        case 0:
            lasers[laser_index].x = mode_info.XResolution / 2 - 7;
            lasers[laser_index].y = (mode_info.YResolution / 2) - (player->height / 2) - 5;
            lasers[laser_index].xspeed = 0;
            lasers[laser_index].yspeed = -10;
            break;
        /*case ARROW_DOWN_BREAK:
            lasers[laser_index].x = mode_info.XResolution / 2;
            lasers[laser_index].y = (mode_info.YResolution / 2) + (player->height / 2);
            lasers[laser_index].xspeed = 0;
            lasers[laser_index].yspeed = 10;
            break;

        case ARROW_LEFT_BREAK:
            lasers[laser_index].x = (mode_info.XResolution / 2) - (player->width / 2);
            lasers[laser_index].y = mode_info.YResolution / 2;
            lasers[laser_index].xspeed = -10;
            lasers[laser_index].yspeed = 0;
            break;

        case ARROW_RIGHT_BREAK:
            lasers[laser_index].x = (mode_info.XResolution / 2) + (player->width / 2);
            lasers[laser_index].y = mode_info.YResolution / 2;
            lasers[laser_index].xspeed = 10;
            lasers[laser_index].yspeed = 0;
            break;*/
        default:
            break;
    }
    laser_index++;
}

void update_lasers() {
    // move lasers
    for (int i = 0; i < laser_index; i++) {
        lasers[i].x += lasers[i].xspeed;
        lasers[i].y += lasers[i].yspeed;
    }

    // check if laser is out of bounds
    for (int i = 0; i < laser_index; i++) {
        if (lasers[i].y < 0 || lasers[i].y > mode_info.YResolution || lasers[i].x < 0 || lasers[i].x > mode_info.XResolution) {
            for (int j = i; j < laser_index - 1; j++) {
                lasers[j] = lasers[j + 1];
            }
            laser_index--;
        }
    }
}

void draw_lasers() {
    // draw lasers
    for (int i = 0; i < laser_index; i++) {
        draw_sprite(laser, lasers[i].x, lasers[i].y);
    }
}

void draw_cannon() {
    switch (selected_cannon) {
        case 0:
            draw_sprite(cannon_up, 380, 227);
            break;
        case 1:
            draw_sprite(cannon_diag1, 415, 240);
            break;
        case 2:
            draw_sprite(cannon_right, 431, 278);
            break;
        case 3:
            draw_sprite(cannon_diag2, 420, 320);
            break;
        case 4:
            draw_sprite(cannon_down, 382, 332);
            break;
        case 5:
            draw_sprite(cannon_diag3, 342, 315);
            break;
        case 6:
            draw_sprite(cannon_left, 328, 278);
            break;
        case 7:
            draw_sprite(cannon_diag4, 345, 242);
            break;
        default:
            break;
    }
}
