#include <lcom/lcf.h>

#include "sprite.h"

Sprite *menu;
Sprite *player;
Sprite *background;
Sprite *laser;

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
    player = create_sprite((xpm_map_t) player_xpm);
    background = create_sprite((xpm_map_t) background_xpm);
    laser = create_sprite((xpm_map_t) laser_xpm);
}

void destroy_xpms() {
    destroy_sprite(menu);
    destroy_sprite(player);
    destroy_sprite(background);
    destroy_sprite(laser);
}
