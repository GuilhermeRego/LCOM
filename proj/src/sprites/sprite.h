#include <lcom/lcf.h>

#include "../imgs/menu.xpm"
#include "../imgs/player.xpm"
#include "../imgs/background.xpm"
#include "../imgs/laser.xpm"
#include "../imgs/cannon/cannon_up.xpm"
#include "../imgs/cannon/cannon_right.xpm"
#include "../imgs/cannon/cannon_down.xpm"
#include "../imgs/cannon/cannon_left.xpm"
#include "../imgs/cannon/cannon_diag1.xpm"
#include "../imgs/cannon/cannon_diag2.xpm"
#include "../imgs/cannon/cannon_diag3.xpm"
#include "../imgs/cannon/cannon_diag4.xpm"
#include "../imgs/asteroid.xpm"

#include "../imgs/letters/char_a.xpm"
#include "../imgs/letters/char_b.xpm"

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
