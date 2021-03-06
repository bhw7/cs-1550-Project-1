//
// Created by School on 6/4/16.
//

#include "library.h"
#include "color.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <linux/fb.h>
#include <sys/termios.h>

struct singleton {
    char sharedData[256];
    unsigned short *frame_buffer;
    void *addr;
    size_t len;
    int prot;
    int flags;
    int fildes;
    off_t off;
    size_t horizontal;
    size_t vertical;
};

struct termio original_termio;

#define FRAME_BUFFER_FILE_DESCRIPTOR "/dev/fb0"

struct singleton *get_instance() {
    static struct singleton *instance = NULL;

    if (instance == NULL) {

        // get map for struct
        instance = mmap(NULL, sizeof(struct singleton), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, 0, 0);

        instance->fildes = open(FRAME_BUFFER_FILE_DESCRIPTOR, O_RDWR);//| O_WRONLY | O_APPEND
        instance->prot = PROT_READ | PROT_WRITE;
        instance->flags = MAP_SHARED;

        // get horizontal and vertical
        struct fb_fix_screeninfo fixed_info;
        struct fb_var_screeninfo screen_info;

        if (!ioctl(instance->fildes, FBIOGET_FSCREENINFO, &fixed_info)) {
            instance->horizontal = (fixed_info.line_length/2);
        }

        if (!ioctl(instance->fildes, FBIOGET_VSCREENINFO, &screen_info)) {
            instance->vertical = screen_info.yres_virtual;
        }

        instance->len = instance->horizontal * instance->vertical * 2;

        instance->frame_buffer = (unsigned short *) mmap(NULL, instance->len,
                                                         instance->prot,
                                                         instance->flags, instance->fildes, 0);

    }

    return instance;
}


void destruct_instance(struct singleton *pSingleton) {

    if (close(pSingleton->fildes) != 0) {
        // error
    }
    // unmap memory
}

void init_graphics() {
    //Settings for stdin (source: svgalib):
    int fd = 0;
    struct termio zap;
    ioctl(fd, TCGETA, &original_termio);
    zap = original_termio;
    zap.c_cc[VMIN] = 0;
    zap.c_cc[VTIME] = 0;
    zap.c_lflag = 0;
    ioctl(fd, TCSETA, &zap);
//    clear_screen();
    color_whole_screen(White);

}

size_t get_frame_buffer_len() {
    struct singleton *instance = get_instance();
    return instance->len;
}

size_t get_horizontal_screen_size() {
    struct singleton *instance = get_instance();
    return instance->horizontal;
}

size_t get_vertical_screen_size() {
    struct singleton *instance = get_instance();
    return instance->vertical;
}

void exit_graphics() {
    //Restore original stdin
    int fd = 0;
    ioctl(fd, TCSETA, &original_termio);

    clear_screen();
    struct singleton *instance = get_instance();
    destruct_instance(instance);
}


int open_file_descriptor() {
    struct singleton *instance = get_instance();
    return instance->fildes;
}

#define MS_TO_SLEEP 500

int write_to_frame_buffer(unsigned short *write_buffer, int num_bytes) {

    unsigned short *fb = get_frame_buffer();

    int i;
    for (i = 0; i < num_bytes; ++i) {
        fb[i] = write_buffer[i];
    }

    sleep_ms(MS_TO_SLEEP);

    return i;
}

unsigned short *get_frame_buffer() {
    struct singleton *instance = get_instance();
    return instance->frame_buffer;
}

// implementation from http://cc.byexamples.com/2007/05/25/nanosleep-is-better-than-sleep-and-usleep/
void sleep_ms(long ms) {
    struct timespec request = {0};

    time_t sec = (int) (ms / 1000);
    ms = ms - (sec * 1000);

    request.tv_sec = sec;
    request.tv_nsec = ms * 1000000L;

    while (nanosleep(&request, &request) == -1 && errno == EINTR)
        continue;
}

void clear_screen() {
    const char msg[] = "\033[2J";
    syscall(4, STDOUT_FILENO, msg, sizeof(msg) - 1);
}


char getkey() {
    int fd = 0;

    char c = '\0';
    int e = (int) read(fd, &c, 1);
    if (e == 0) c = '\0';

    return c;
};

