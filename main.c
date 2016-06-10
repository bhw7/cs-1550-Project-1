//
// Created by Betsalel Williamson on 6/4/16.
//

#include <stdio.h>
#include <signal.h>
#include <sys/param.h>
#include <stdlib.h>
#include <lib/include/draw.h>
#include "library.h"
#include "color.h"

/**
 * Handle quit to correctly exit and restore state on CTR-C.
 */
void sig_handler(int signo) {
    printf("\n\nProcessing received signal: %d\n", signo);

    exit_graphics();

    if (signo == SIGINT) {
        exit(EXIT_SUCCESS);
    }
    // else
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

    init_graphics();

//    draw_sample_colors();

//    draw_rect(20, 20, 100, 40, Orange);
//
//    sleep_ms(1000);

    draw_text(40, 40, "abc", Red);

    draw_text(636, 476, "def", Orange);

    sleep_ms(5000);

    exit_graphics();

    exit(EXIT_SUCCESS);
}