#include <lcom/lcf.h>

typedef enum {
    MAIN_MENU,
    GAME,
    GAME_OVER,
    HIGHSCORES,
    SETTINGS,
    EXIT
} State;

State state = MAIN_MENU;

int (main)(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
    lcf_log_output("/home/lcom/labs/proj/output.txt");
    if (lcf_start(argc, argv)) return 1;

    lcf_cleanup();

    return 0;
}

int (proj_main_loop)(int argc, char **argv) {
    /* your main loop code here */
    return 0;
}
