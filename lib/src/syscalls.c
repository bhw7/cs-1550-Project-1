//
// Created by School on 6/4/16.
//

#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include "syscalls.h"

void init_graphics() {

    puts("Hello");

}

int open_frame_buffer(int options) {
    return open("/dev/fb0", options);
}