#include <lcom/lcf.h>

#include "game/game.h"

extern GameState gameState;
extern int timer_cnt;
extern uint8_t scancode;

int run_game() {
    if (mouse_write(SET_STREAM_MODE) != 0) return 1;
    if (mouse_write(ENABLE_DATA_REPORT) != 0) return 1;
    
    if (timer_set_frequency(0, 19) != 0) return 1;

    int ipc_status;
    message msg;
    uint8_t irq_timer, irq_mouse, irq_keyboard;

    if (timer_subscribe_int(&irq_timer) != 0) return 1;
    if (mouse_subscribe_int(&irq_mouse) != 0) return 1;
    if (keyboard_subscribe_int(&irq_keyboard) != 0) return 1;

    while (gameState != EXIT) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            printf("Error driver_receive");
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_timer) {
                        timer_int_handler();
                        switch (gameState) {
                            // Create for each state a function that draws the state
                            case MAIN_MENU:
                                draw_menu();
                                break;
                            case GAME:
                                // TODO
                                break;
                            case GAME_OVER:
                                // TODO
                                break;
                            case EXIT:
                                break;
                        }
                        swap_buffers();
                    }

                    if (msg.m_notify.interrupts & irq_keyboard) {
                        kbc_ih();
                        interpret_scancode();
                    }

                    if (msg.m_notify.interrupts & irq_mouse) {
                        // TODO
                        continue;
                    }
                    break;
                default:
                    break;
            }
        }
    } 

    if (timer_unsubscribe_int() != 0) return 1;
    if (mouse_unsubscribe_int() != 0) return 1;
    if (keyboard_unsubscribe_int() != 0) return 1;

    if (mouse_write(DISABLE_DATA_REPORT) != 0) return 1;

    return 0;
}

void interpret_scancode() {
    switch (gameState) {
        case MAIN_MENU: {
            switch (scancode) {
                case ESC_BREAK:
                    gameState = EXIT;
                    break;
                case ARROW_UP_BREAK:
                    printf("UP\n");
                    break;
                case ARROW_DOWN_BREAK:
                    printf("DOWN\n");
                    break;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
}
