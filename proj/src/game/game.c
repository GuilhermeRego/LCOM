#include <lcom/lcf.h>

#include "game/game.h"


extern GameState gameState;

int run_game() {
    if (mouse_write(SET_STREAM_MODE) != 0) return 1;
    if (mouse_write(ENABLE_DATA_REPORT) != 0) return 1;
    
    if (timer_set_frequency(0, 30) != 0) return 1;

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
                                vg_draw_rectangle(0, 0, 300, 300, 0xffffff);
                                vg_draw_rectangle(0, 0, 100, 100, 0xffa150);
                                sleep(10);
                                gameState = EXIT;
                                break;
                            case GAME:
                                // TODO
                                break;
                            case GAME_OVER:
                                // TODO
                                break;
                            case EXIT:
                                break;
                            default:
                                return 1;
                        }
                    }
                    if (msg.m_notify.interrupts & irq_mouse) {
                        // TODO
                        continue;
                    }
                    if (msg.m_notify.interrupts & irq_keyboard) {
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
