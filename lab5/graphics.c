#include "graphics.h"

int (set_graphic_mode)(uint16_t submode) {
    reg86_t r86;
    memset(&r86, 0, sizeof(r86));
    r86.intno = 0x10;
    r86.ax = 0x4F02; // 0x42 -> VBE Call; 0x02 -> Set Video Graphics Mode;
    r86.bx = submode | VBE_LINEAR_FB;
    if (sys_int86(&r86) != 0) {
        printf("Set VBE Mode Failed!\n");
        return 1;
    }
    return 0;
}

int (set_text_mode)() {
    reg86_t r86;
    memset(&r86, 0, sizeof(r86));
    r86.intno = 0x10;
    r86.ax = 0x0003; // 0x03 -> Text Mode
    r86.bx = 0x0000; // No Submode in Text Mode
    if (sys_int86(&r86) != 0) {
        printf("Set Text Mode Failed!\n");
        return 1;
    }
    return 0;
}

int (set_frame_buffer)(uint16_t mode) {
    memset(&mode_info, 0, sizeof(mode_info));
    vbe_get_mode_info(mode, &mode_info);

    bytes_pp = (mode_info.BitsPerPixel + 7) / 8; // Bytes Per Pixel
    uint fsize_pixels = mode_info.XResolution * mode_info.YResolution;
    uint fsize_bytes = fsize_pixels * bytes_pp;

    struct minix_mem_range range;
    range.mr_base = mode_info.PhysBasePtr;
    range.mr_limit = mode_info.PhysBasePtr + fsize_bytes;

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &range)) {
        printf("Memory Range Alocation Error\n");
        return 1;
    }

    frame_buffer = vm_map_phys(SELF, (void*) range.mr_base, fsize_bytes);
    if (frame_buffer == NULL) {
        printf("Virtual Memory Allocation Error\n");
        return 1;
    }

    return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {

    if (x > mode_info.XResolution || y > mode_info.YResolution) return 1; // Verificar se pos do pixel e valida.

    uint position = (mode_info.XResolution * y + x) * bytes_pp;

    if (memcpy(&frame_buffer[position], &color, bytes_pp) == NULL) return 1;
    
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (uint i = 0; i < len; i++) {
        if (vg_draw_pixel(x + i, y, color) != 0) {
            vg_exit();
            return 1;
        }
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (uint i = 0; i < height; i++) {
        if (vg_draw_hline(x, y + i, width, color) != 0) {
            vg_exit();
            return 1;
        }
    }
    return 0;
} 

// Bitmsak = BIT(15) - 1 = 0b0111111111111111
int normalize_color(uint32_t color, uint32_t *new_color) {
    *new_color = color & (BIT(mode_info.BitsPerPixel) - 1);
    return 0;
}

/* int (normalize_color)(uint32_t color, uint32_t *new_color) {
    uint8_t red = (color >> 16) & 0xFF;
    uint8_t green = (color >> 8) & 0xFF;
    uint8_t blue = color & 0xFF;
    switch (mode_info.BitsPerPixel) {
        // 1024x768
        case 8: {
            *new_color = color & 0x0000000F;
            return 0;
        }
        // 640x480 15 [(1:) 5 : 5 : 5]
        case 15: {
            red = (red & 0x1F) << 10;
            green = (green & 0x1F) << 5;
            blue = (blue & 0x1F);
            *new_color = red | green | blue;
            return 0;
        }
        // 800x600 24 [8 : 8 : 8]
        case 24: {
            red = red << 16;
            green = green << 8;
            *new_color = red | green | blue;
            return 0;
        }
        // 1280x1024 16 [5 : 6 : 5]
        case 16 : {
            red = (red & 0x1F) << 11;
            green = (green & 0x3F) << 5;
            blue = (blue & 0x1F);
            *new_color = red | green | blue;
            return 0;
        }
        // 1152x864 32 [(8:) 8 : 8 : 8]
        case 32: {
            red = red << 16;
            green = green  8;
            *new_color = red | green | blue;
            return 0;
        }
        default:
            return 1;
    }
    return 1;
} */

