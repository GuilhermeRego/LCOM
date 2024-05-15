#include <lcom/lcf.h>

#include "sprite.h"

Sprite *menu_title;
Sprite *test;

Sprite *create_sprite(xpm_map_t pic) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    xpm_image_t img;

    if( sp == NULL )
        return NULL;
    // read the sprite pixmap
    sp->map = (uint32_t *) xpm_load(pic, XPM_8_8_8_8, &img);
    if(sp->map == NULL) {
        free(sp);
        return NULL;
    }
    sp->width = img.width;
    sp->height = img.height;

    return sp;
}

void destroy_sprite(Sprite *sp) {
    if( sp == NULL ) return;
    if( sp ->map ) free(sp->map);
    free(sp);
    sp = NULL;
}

int draw_sprite(Sprite *sp, int x, int y) {
    if (sp == NULL) return 1;
    for (int i = 0; i < sp->height; i++) {
        for (int j = 0; j < sp->width; j++) {
            if (vg_draw_pixel(x + j, y + i, sp->map[i * sp->width + j]) != 0) return 1;
        }
    }
    return 0;
}

void load_xpms() {
    menu_title = create_sprite((xpm_map_t) menu_title_xpm);
    test = create_sprite((xpm_map_t) test_xpm);
}

void destroy_xpms() {
    destroy_menu_sprites();
}

void destroy_menu_sprites() {
    destroy_sprite(menu_title);
}
