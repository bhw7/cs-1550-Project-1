//
// Created by Betsalel Williamson on 6/4/16.
//

#include <stdio.h>
#include "syscalls.h"

int main(int argc, char **argv) {

    unsigned short buffer[1] = {0xffff};

    write_to_frame_buffer(buffer, 1);
    puts("hello");
    return 0;
}