#include <lcom/lcf.h>

#include "mouse_controller.h"

void draw_mouse() {
    if (pp.delta_x != 0 || pp.delta_y != 0) {
        if (cursor->x + pp.delta_x < 0) cursor->x = 0;
        else if (cursor->x + pp.delta_x + cursor->width >= mode_info.XResolution) cursor->x = mode_info.XResolution - cursor->width;
        else cursor->x += pp.delta_x;

        if (cursor->y - pp.delta_y < 0) cursor->y = 0;
        else if (cursor->y - pp.delta_y + cursor->height >= mode_info.YResolution) cursor->y = mode_info.YResolution - cursor->height;
        else cursor->y -= pp.delta_y;
    }
    printf("x: %d, y: %d\n", cursor->x, cursor->y);
    draw_sprite(cursor, cursor->x, cursor->y);
}
