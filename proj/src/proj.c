#include <lcom/lcf.h>

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
