#ifndef __VBE_H
#define __VBE_H

#define VBE_MODE_768p 0x105 // Indexed Bits per Pixel: 8
#define VBE_MODE_480p 0x110 // Direct Color Bits per Pixel: 15 [(1:) 5 : 5 : 5]
#define VBE_MODE_600p 0x115 // Direct Color Bits per Pixel: 24 [8 : 8 : 8]
#define VBE_MODE_1024p 0x11A // Direct Color Bits per Pixel: 16 [5 : 6 : 5]
#define VBE_MODE_864p 0x14C // Direct Color Bits per Pixel: 32 [(8:) 8 : 8 : 8]

#define VBE_MODE_SET 0x4F02
#define VBE_LINEAR_FB BIT(14)

#define DIRECT_COLOR 0x06
#define INDEXED_COLOR 0x105

#define BIOS_SET_VIDEOMODE 0x00
#define BIOS_VIDEOCARD_SERV 0x10

#endif
