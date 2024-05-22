#include <lcom/lcf.h>

#include "game/game.h"

extern GameState gameState;
extern int timer_cnt;
extern uint8_t scancode;
extern int option;
extern laser_t lasers[100];
extern int laser_index;
extern int selected_cannon;
extern int ammo;
extern bool out_of_ammo;
int freq = 40;

int run_game() {
    if (mouse_write(SET_STREAM_MODE) != 0) return 1;
    if (mouse_write(ENABLE_DATA_REPORT) != 0) return 1;
    
    if (timer_set_frequency(0, 60) != 0) return 1;

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
                            case MENU:
                                draw_menu();
                                break;
                            case GAME:
                                update_lasers();
                                update_asteroids();
                                update_difficulty();
                                if (timer_cnt % freq == 0) create_asteroid();
                                draw_game();
                                break;
                            case SETTINGS:
                                printf("Settings running\n");
                                break;
                            case INSTRUCTIONS:
                                printf("Instructions running\n");
                                break;
                            case EXIT:
                                return 0;
                            case GAME_OVER:
                                reset_game();
                                gameState = MENU;
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
        case MENU: {
            switch (scancode) {
                case ESC_BREAK:
                    gameState = EXIT;
                    break;
                case ARROW_UP_BREAK:
                    if (option > 0) option--;
                    else option = 3;
                    break;
                case ARROW_DOWN_BREAK:
                    if (option < 3) option++;
                    else option = 0;
                    break;
                case ENTER_BREAK:
                    switch (option) {
                        case 0:
                            gameState = GAME;
                            break;
                        case 1:
                            gameState = SETTINGS;
                            break;
                        case 2:
                            gameState = INSTRUCTIONS;
                            break;
                        case 3:
                            gameState = EXIT;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        }

        case GAME: {
            switch (scancode) {
                case ESC_BREAK:
                    gameState = MENU;
                    reset_game();
                    break;
                case ARROW_LEFT_BREAK:
                    selected_cannon--;
                    if (selected_cannon < 0) selected_cannon = 7;
                    break;
                case ARROW_RIGHT_BREAK:
                    selected_cannon++;
                    if (selected_cannon > 7) selected_cannon = 0;
                    break;
                case SPACE_BREAK:
                    if (!out_of_ammo)
                        create_laser();
                    break;
                case ARROW_DOWN_BREAK:
                    out_of_ammo = false;
                    ammo = 5;
                    break;
                default:
                    break;
            }
            break;
        }

        case SETTINGS: {
            if (scancode == ESC_BREAK) gameState = MENU;
            break;
        }

        case INSTRUCTIONS: {
            if (scancode == ESC_BREAK) gameState = MENU;
            break;
        }

        default:
            break;
    }
}
