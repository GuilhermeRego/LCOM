#include <lcom/lcf.h>

#include "utils/video/VBE_macros.h"
#include "utils/video/video.c"

#include "utils/keyboardMouse/i8042.h"
#include "utils/keyboardMouse/keyboard.c"
#include "utils/keyboardMouse/mouse.c"

#include "utils/timer/i8254.h"
#include "utils/timer/timer.c"

#include "sprites/sprite.c"

#include "gameStates/menu.c"
#include "gameStates/game_controller.c"

#include "difficulty_controller.c"

#include "gameStates/mouse_controller.c"

int run_game();

void interpret_scancode();
