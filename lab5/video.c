#include <lcom/lcf.h>

#include "VBE.h"
#include "video.h"

vbe_mode_info_t mode_info;
uint8_t *video_mem;

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

int (config_frame_buffer)(uint16_t mode) {
    // Get mode info
    memset(&mode_info, 0, sizeof(mode_info));
    if (vbe_get_mode_info(mode, &mode_info) != 0) return 1;
    
    // Allow memory mapping
    int r, vram_size = mode_info.XResolution * mode_info.YResolution * mode_info.BitsPerPixel;
    struct minix_mem_range mr;
    mr.mr_base = mode_info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vram_size;
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return 1;
    }

    // Map memory
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED) {
        panic("couldn’t map video memory");
        return 1;
    }

    return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if (x > mode_info.XResolution || y > mode_info.YResolution) return 1;

    // Calculate bytes per pixel (divide by 8) and the index in bytes
    int bytesPerPixel = mode_info.BitsPerPixel / 8;
    int index =  bytesPerPixel * (y * mode_info.XResolution + x);

    // Copy to memory
    if (memcpy(&video_mem[index], &color, bytesPerPixel) == NULL) return 1;
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (int i = 0; i < len; i++) {
        if (vg_draw_pixel(x+i, y, color) != 0) return 1;
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (int i = 0; i < height; i++) {
        if (vg_draw_hline(x, y+i, width, color) != 0) return 1;
    }
    return 0;
}

int (transform_color)(uint32_t color, uint32_t *new_color) {
    uint8_t red, green, blue;
    switch (mode_info.BitsPerPixel) {
        // 1024x768
        case 8: {
            *new_color = color & 0x0000000F;
            return 0;
        }
        // 640x480
        case 15: {
            red = (color & 0x00007C00) >> 8;
            green = (color & 0x000003E0) >> 4;
            blue = (color & 0x0000001F);
            *new_color = red | green | blue;
            return 0;
        }
        // 800x600
        case 24: {
            red = (color & 0x00FF0000) >> 16;
            green = (color & 0x0000FF00) >> 8;
            blue = (color & 0x000000FF);
            *new_color = red | green | blue;
            return 0;
        }
        // 1280x1024
        case 16 : {
            red = (color & 0x0000F800) >> 8;
            green = (color & 0x000007E0) >> 3;
            blue = (color & 0x0000001F);
            *new_color = red | green | blue;
            return 0;
        }
        // 1152x864
        case 32: {
            red = (color & 0x00FF0000) >> 16;
            green = (color & 0x0000FF00) >> 8;
            blue = (color & 0x000000FF);
            *new_color = red | green | blue;
            return 0;
        }
        default:
            return 1;
    }
    return 1;
}

uint32_t (direct_mode) (uint32_t red, uint32_t green, uint32_t blue) {
    return (red << mode_info.RedFieldPosition) | (green << mode_info.GreenFieldPosition) | (blue << mode_info.BlueFieldPosition);
}

uint32_t (index_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t no_rectangles) {
  return (first + (row * no_rectangles + col) * step) % (1 << mode_info.BitsPerPixel);
}

uint32_t (Red)(int j, uint8_t step, uint32_t first) {
    return (R(first) + j * step) % (1 << mode_info.RedMaskSize);
}

uint32_t (Green)(int i, uint8_t step, uint32_t first) {
    return (G(first) + i * step) % (1 << mode_info.GreenMaskSize);
}

uint32_t (Blue)(int j, int i, uint8_t step, uint32_t first) {
    return (B(first) + (i+j) * step) % (1 << mode_info.BlueMaskSize);
}

uint32_t (R)(int first) {
  return ((1 << mode_info.RedMaskSize) - 1) & (first >> mode_info.RedFieldPosition);
}

uint32_t (G)(int first) {
  return ((1 << mode_info.GreenMaskSize) - 1) & (first >> mode_info.GreenFieldPosition);
}

uint32_t (B)(int first) {
  return ((1 << mode_info.BlueMaskSize) - 1) & (first >> mode_info.BlueFieldPosition);
}

int (vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step) {
    uint16_t width = mode_info.XResolution / no_rectangles;
    uint16_t height = mode_info.YResolution / no_rectangles;
    for (int i = 0; i < no_rectangles; i++) {
        for (int j = 0; j < no_rectangles; j++) {
            uint32_t color;
            switch (mode_info.BitsPerPixel) {
                case 15: case 16: case 24: case 32: {   // Direct mode
                    uint32_t red = Red(j, step, first);
                    uint32_t green = Green(i, step, first);
                    uint32_t blue = Blue(j, i, step, first);
                    color = direct_mode(red, green, blue);
                    break;
                }
                case 8: {   // Indexed mode
                    color = index_mode(j, i, step, first, no_rectangles);
                    break;
                }
                default:
                    return 1;
            }
            if(vg_draw_rectangle(j * width, i * height, width, height, color) != 0) return 1;
        }
    }
    
    return 0;
}

int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    // We need to fill the xpm_load function with its parameters: xpm_map_t map, enum xpm_image_type type, xpm_image_t *img
    xpm_image_t img;
    uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
    int index = 0;
    if (colors == NULL) return 1;
    for (int j = 0; j < img.height; j++) {
        for (int i = 0; i < img.width; i++) {
            if (vg_draw_pixel(i + x, j + y, colors[index]) != 0) return 1;
            index++;
        }
    }

    return 0;
}

int (move_xpm)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
    for (int i = xi; i != xf; i += speed) {
        for (int j = yi; j != yf; j += speed) {
            if (draw_xpm(xpm, i, j) != 0) return 1;
            if (vg_draw_rectangle(i, j, 0, 0, 0) != 0) return 1;
            tickdelay(micros_to_ticks(1000000 / fr_rate));
        }
    }
    return 0;
}
