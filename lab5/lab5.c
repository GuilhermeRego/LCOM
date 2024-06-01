// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include <lcom/pixmap.h>

#include <stdint.h>
#include <stdio.h>
#include "grafica.h"
#include "keyboard.h"
#include "Kbc.h"
#include "i8042.h"
#include "timer.c"

extern vbe_mode_info_t modeinf;
extern int counter;
extern uint8_t scanc;


// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (waiting_ESC_key)() {

  int ipc_status;
  message msg;
  uint8_t keyboardid;

  if (kb_subscribe_int(&keyboardid) != 0) return 1;

  while (scanc != Esc_break){
    if (driver_receive(ANY, &msg, &ipc_status) != 0) { 
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & keyboardid) 
            kbc_ih();
            break;
        default:
          break; 
      }
    }
  }

  if (kb_unsubscribe_int() != 0) return 1;
  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  
  if (set_graphic(mode) != 0){
     return 1;
  }

  sleep(delay);

  
  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  if(set_frmbuff(mode) != 0){
    return 1;
  }

  if(set_graphic(mode) != 0){
    return 1;
  }

  uint32_t new_color;
  if (normalize_color(color, &new_color) != 0) {
    return 1;
  }

  if(vg_draw_rectangle(x, y, width,height , new_color) != 0){
    return 1;
  }

  if(waiting_ESC_key() != 0){
    return 1;
  }

  if(vg_exit() != 0){
    return 1;
  }
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
 
  if (set_frmbuff(mode) != 0) return 1;

  if (set_graphic(mode) != 0) return 1;

  
  int vert = modeinf.YResolution / no_rectangles;
  int hori = modeinf.XResolution / no_rectangles;

  for (int d = 0 ; d < no_rectangles ; d++) {
    for (int b = 0 ; b < no_rectangles ; b++) {

      uint32_t color;

      if (modeinf.MemoryModel == DCOLOR) {
        uint32_t R = Red(b, step, first);
        uint32_t G = Green(d, step, first);
        uint32_t B = Blue(b, d, step, first);
        color = direct_mode(R, G, B);
        

      } else {
        color = indexed_mode(b, d, step, first, no_rectangles);
      }

      if (vg_draw_rectangle(b * hori, d * vert, hori, vert, color)) {
        return 1;
      }
    }
  }
  
  if (waiting_ESC_key()) return 1;

 
  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if(set_frmbuff(g768p_IND) != 0){
    return 1;
  }

  if(set_graphic(g768p_IND) != 0){
    return 1;
  }


   if(show_xpm(xpm, x, y) != 0){
    return 1;
  }

  if(waiting_ESC_key() != 0){
    return 1;
  }

  if(vg_exit() != 0){
    return 1;
  }

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
