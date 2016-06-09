//
// Created by Merlin on 16/11/15.
//

#include <fcntl.h>
#include <sys/ioctl.h>
#include "syscalls.h"
#include "gtest/gtest.h"
#include "mother.h"

#include <linux/fb.h>


TEST(BasicTest, Open_zeroth_framebuffer) {
    // opening this file will work

    int filedesc = open_frame_buffer(O_WRONLY | O_APPEND);

    printf("Opened filedesc: %d\n", filedesc);

    ASSERT_GT(filedesc, 0);
}

#define BYTES_TO_READ 32

TEST(BasicTest, Write_to_zeroth_framebuffer) {

    unsigned short writebuffer_size_1[1] = {0x0005};

    ssize_t bytesWritten = write_to_frame_buffer(writebuffer_size_1, 1);

    mother::print_buffer(read_frame_buffer(BYTES_TO_READ), BYTES_TO_READ);
    ASSERT_EQ(bytesWritten, 1);

    unsigned short writebuffer_size_5[5] = {0xabcd, 0xef01, 0x2345, 0x6789, 0xabcd};

    bytesWritten = write_to_frame_buffer(writebuffer_size_5, 10);

    mother::print_buffer(read_frame_buffer(BYTES_TO_READ), BYTES_TO_READ);
    ASSERT_EQ(bytesWritten, 10);
}

TEST(BasicTest, Get_screen_size) {

    int x = get_screen_size();

    struct fb_var_screeninfo screen_info;
    struct fb_fix_screeninfo fixed_info;
    char *buffer = NULL;
    size_t buflen;
    int fd = -1;
    int r = 1;

    fd = open("/dev/fb0", O_RDWR);
    if (fd >= 0) {
        if (!ioctl(fd, FBIOGET_VSCREENINFO, &screen_info) &&
            !ioctl(fd, FBIOGET_FSCREENINFO, &fixed_info)) {
            buflen = screen_info.yres_virtual * fixed_info.line_length;

            printf("%dx%d or %d\n", fixed_info.line_length, screen_info.yres_virtual, (int) buflen);

        }
        else {
            perror("open");
        }

        if (fd >= 0)
            close(fd);

    }
}