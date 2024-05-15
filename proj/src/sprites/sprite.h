#include <lcom/lcf.h>

#include "../imgs/menu_title.xpm"
#include "../imgs/test.xpm"

typedef struct {
    int x, y; // current position
    int width, height; // dimensions
    int xspeed, yspeed; // current speed
    uint32_t *map; // the pixmap
} Sprite;

Sprite *create_sprite(xpm_map_t pic);

void destroy_sprite(Sprite *sp);

void load_xpms();

void destroy_xpms();

int draw_sprite(Sprite *sp, int x, int y);

void destroy_menu_sprites();
