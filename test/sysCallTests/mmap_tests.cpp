//
// Created by School on 6/7/16.
//

#include <fcntl.h>
#include <sys/mman.h>
#include "library.h"
#include "gtest/gtest.h"
#include "mother.h"

#define BYTES_TO_READ 32

TEST(Mmap_tests, Mmap_read) {

    unsigned short *fb = get_frame_buffer();
    mother::print_buffer(fb, BYTES_TO_READ);

}

//TEST(Mmap_tests, Mmap_write) {
//    EXPECT_EQ(true, true);
//}
//
//TEST(Mmap_tests, Mmap_read_and_write) {
//    EXPECT_EQ(true, true);
//}

//#define handle_error(msg) \
//           do { perror(msg); exit(EXIT_FAILURE); } while (0)
//
//int
//main(int argc, char *argv[])
//{
//    char *addr;
//    int fd;
//    struct stat sb;
//    off_t offset, pa_offset;
//    size_t length;
//    ssize_t s;
//
//    if (argc < 3 || argc > 4) {
//        fprintf(stderr, "%s file offset [length]\n", argv[0]);
//        exit(EXIT_FAILURE);
//    }
//
//    fd = open(argv[1], O_RDONLY);
//    if (fd == -1)
//        handle_error("open");
//
//    if (fstat(fd, &sb) == -1)           /* To obtain file size */
//        handle_error("fstat");
//
//    offset = atoi(argv[2]);
//    pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
//    /* offset for mmap() must be page aligned */
//
//    if (offset >= sb.st_size) {
//        fprintf(stderr, "offset is past end of file\n");
//        exit(EXIT_FAILURE);
//    }
//
//    if (argc == 4) {
//        length = (size_t) atoi(argv[3]);
//        if (offset + length > sb.st_size)
//            length = (size_t) (sb.st_size - offset);
//        /* Can't display bytes past end of file */
//
//    } else {    /* No length arg ==> display to end of file */
//        length = (size_t) (sb.st_size - offset);
//    }
//
//    addr = (char *) mmap(NULL, length + offset - pa_offset, PROT_READ | PROT_WRITE,
//                         MAP_PRIVATE, fd, pa_offset);
//    if (addr == MAP_FAILED)
//        handle_error("mmap");
//
//    s = write(STDOUT_FILENO, addr + offset - pa_offset, length);
//    if (s != length) {
//        if (s == -1)
//            handle_error("write");
//
//        fprintf(stderr, "partial write");
//        exit(EXIT_FAILURE);
//    }
//
//    exit(EXIT_SUCCESS);
//}