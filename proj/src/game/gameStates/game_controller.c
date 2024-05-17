#include <lcom/lcf.h>

#include "game_controller.h"

typedef struct {
    int x, y;
    int xspeed, yspeed;
} laser_t;

laser_t lasers[100];
int laser_index = 0;

void draw_lasers();

void draw_game() {
    draw_sprite(background, 0, 0);
    draw_sprite(player, player->x, player->y);
    draw_lasers();
}

void create_laser(uint8_t key) {
    switch (key) {
        case ARROW_UP_BREAK:
            lasers[laser_index].x = mode_info.XResolution / 2 - 7;
            lasers[laser_index].y = (mode_info.YResolution / 2) - (player->height / 2) + 15;
            lasers[laser_index].xspeed = 0;
            lasers[laser_index].yspeed = -10;
            break;
        case ARROW_DOWN_BREAK:
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
            break;
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
