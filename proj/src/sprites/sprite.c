#include <lcom/lcf.h>

#include "sprite.h"

Sprite *menu;
Sprite *player;
Sprite *background;
Sprite *laser;
Sprite *cannon_up;
Sprite *cannon_right;
Sprite *cannon_down;
Sprite *cannon_left;
Sprite *cannon_diag1;
Sprite *cannon_diag2;
Sprite *cannon_diag3;
Sprite *cannon_diag4;
Sprite *asteroid;

Sprite *create_sprite(xpm_map_t pic) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    xpm_image_t img;

    // read the sprite pixmap
    sp->map = (uint32_t *) xpm_load(pic, XPM_8_8_8_8, &img);
    if(sp->map == NULL) {
        printf("create_sprite: xpm_load failed\n");
        free(sp);
        return NULL;
    }
    sp->width = img.width;
    sp->height = img.height;

    sp->x = 0;
    sp->y = 0;

    sp->xspeed = 0;
    sp->yspeed = 0;

    return sp;
}

void destroy_sprite(Sprite *sp) {
    if( sp == NULL ) return;
    if( sp ->map ) free(sp->map);
    free(sp);
    sp = NULL;
}

int draw_sprite(Sprite *sp, int x, int y) {
    if (sp == NULL) {
        printf("draw_sprite: sp is NULL\n");
        return 1;
    }
    for (int i = 0; i < sp->height; i++) {
        for (int j = 0; j < sp->width; j++) {
            if (vg_draw_pixel(x + j, y + i, sp->map[i * sp->width + j]) != 0) return 1;
        }
    }
    return 0;
}

void load_xpms() {
    menu = create_sprite((xpm_map_t) menu_xpm);
    if (menu == NULL) printf("menu is NULL\n");

    player = create_sprite((xpm_map_t) player_xpm);
    if (player == NULL) printf("player is NULL\n");
    player->x = (mode_info.XResolution / 2) - (player->width / 2);
    player->y = (mode_info.YResolution / 2) - (player->height / 2);
    
    background = create_sprite((xpm_map_t) background_xpm);
    if (background == NULL) printf("background is NULL\n");

    laser = create_sprite((xpm_map_t) laser_xpm);
    if (laser == NULL) printf("laser is NULL\n");

    cannon_up = create_sprite((xpm_map_t) cannon_up_xpm);
    if (cannon_up == NULL) printf("cannon_up is NULL\n");

    cannon_right = create_sprite((xpm_map_t) cannon_right_xpm);
    if (cannon_right == NULL) printf("cannon_right is NULL\n");

    cannon_down = create_sprite((xpm_map_t) cannon_down_xpm);
    if (cannon_down == NULL) printf("cannon_down is NULL\n");

    cannon_left = create_sprite((xpm_map_t) cannon_left_xpm);
    if (cannon_left == NULL) printf("cannon_left is NULL\n");

    cannon_diag1 = create_sprite((xpm_map_t) cannon_diag1_xpm);
    if (cannon_diag1 == NULL) printf("cannon_diag1 is NULL\n");

    cannon_diag2 = create_sprite((xpm_map_t) cannon_diag2_xpm);
    if (cannon_diag2 == NULL) printf("cannon_diag2 is NULL\n");

    cannon_diag3 = create_sprite((xpm_map_t) cannon_diag3_xpm);
    if (cannon_diag3 == NULL) printf("cannon_diag3 is NULL\n");

    cannon_diag4 = create_sprite((xpm_map_t) cannon_diag4_xpm);
    if (cannon_diag4 == NULL) printf("cannon_diag4 is NULL\n");

    asteroid = create_sprite((xpm_map_t) asteroid_xpm);
    if (asteroid == NULL) printf("asteroid is NULL\n");
}

void destroy_xpms() {
    destroy_sprite(menu);
    destroy_sprite(player);
    destroy_sprite(background);
    destroy_sprite(laser);
}
