#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef ESTATE
#define ESTATE
struct editor_state {
    struct termios orig_termios;
    size_t rows;
    size_t cols;
};
#endif

int get_window_size(struct editor_state *state);
void enable_raw_mode(struct editor_state *state);
void disable_raw_mode();
void clean_exit(const char *msg);
void die(const char *msg);