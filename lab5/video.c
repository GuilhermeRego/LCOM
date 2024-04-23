#include <lcom/lcf.h>

#include "VBE.h"
#include "video.h"

int (vg_set_mode)(uint16_t mode) {
    reg86_t r86;
        
    memset(&r86, 0, sizeof(r86));
    
    r86.intno = VIDEO_CARD;
    r86.ah = VBE_AH;
    r86.al = GRAPHICS_MODE;
    r86.bx = mode | LINEAR_MODE;

    if (sys_int86(&r86) != OK) {
        printf("\tvg_exit(): sys_int86() failed \n");
        return 1;
    }
    return 0;
}
