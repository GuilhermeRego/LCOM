// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include "video.h"
#include "keyboard.h"
#include "kbc.h"
#include <lcom/pixmap.h>

#include <stdint.h>
#include <stdio.h>

uint8_t scancode;
int cnt = 0;

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

int wait_esc_exit() {
	int ipc_status;
    uint8_t irq_set;
    message msg;

	if (keyboard_subscribe_int(&irq_set) != 0) return 1;

	while (scancode != ESC_BREAKCODE) {		// Enquanto a tecla premida não for ESC, continua a fazer scan

		if (driver_receive(ANY, &msg, &ipc_status) != 0) {printf("Driver\n");return 1;}

		if (is_ipc_notify(ipc_status)) {	// Verifica se recebemos um interrupt
			switch(_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.m_notify.interrupts & irq_set) {
						kbc_ih();
					}
        default:
          break;
			}
		}
	}

	if (keyboard_unsubscribe_int() != 0) return 1;
  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (vg_set_mode(mode) != 0) {printf("erro vg_set_mode");return 1;}
  sleep(delay);
  if (vg_exit() != 0) {printf("erro vg_exit");return 1;}
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  // Map the video memory to the process' adress space
  if (config_frame_buffer(mode) != 0) return 1;

  // Change the video mode
  if (vg_set_mode(mode) != 0) return 1;

  // The color changes depending on the resolution
  uint32_t new_color;
  if (transform_color(color, &new_color) != 0) return 1;
  
  // Draw a rectangle
  if (vg_draw_rectangle(x, y, width, height, color) != 0) return 1;

  // Reset the video mode upon receiving the ESC key
  if (wait_esc_exit() != 0) return 1;
  return 1;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if (config_frame_buffer(mode) != 0) return 1;
  if (vg_set_mode(mode) != 0) return 1;

  if (vg_draw_pattern(no_rectangles, first, step) != 0) return 1;

  if (wait_esc_exit() != 0) return 1;
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if (config_frame_buffer(0x105) != 0) return 1;
  if (vg_set_mode(0x105) != 0) return 1;

  if (draw_xpm(xpm, x, y) != 0) return 1;

  if (wait_esc_exit() != 0) return 1;
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  if(config_frame_buffer(0x105)) return 1;
  if(vg_set_mode(0x105)) return 1;

  if(move_xpm(xpm, xi, yi, xf, yf, speed, fr_rate) != 0) return 1;

  if (vg_exit() != 0) return 1;

  return 0;
}
