#include <lcom/lcf.h>

#include "utils/video/VBE.h"
#include "utils/video/video.h"

#include "utils/timer/i8254.h"
#include "utils/timer/timer.h"


#include "utils/keyboardMouse/i8042.h"
#include "utils/keyboardMouse/keyboard.h"
#include "utils/keyboardMouse/kbc.h"
#include "utils/keyboardMouse/mouse.h"

#include "game/game.h"
#include "utils/utils.c"

GameState gameState = MAIN_MENU;

int (main)(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
    lcf_log_output("/home/lcom/labs/proj/output.txt");
    if (lcf_start(argc, argv)) return 1;

    lcf_cleanup();

    return 0;
}

int (proj_main_loop)(int argc, char **argv) {
    if (config_frame_buffer(VBE_640x480_MODE) != 0) return 1;
    if (vg_set_mode(VBE_640x480_MODE) != 0) return 1;

    if (run_game() != 0) return 1;
    
    if (vg_exit() != 0) return 1;
    return 0;
}
