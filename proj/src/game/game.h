#include <lcom/lcf.h>
#include "utils/video/VBE_macros.h"
#include "utils/video/video.c"
#include "utils/keyboardMouse/i8042.h"
#include "utils/keyboardMouse/keyboard.c"
#include "utils/keyboardMouse/mouse.c"

#include "utils/timer/i8254.h"
#include "utils/timer/timer.c"

typedef enum {
    MAIN_MENU,
    GAME,
    GAME_OVER,
    EXIT
} GameState;

int run_game();
