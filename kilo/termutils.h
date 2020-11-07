#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#ifndef ESTATE
    #define ESTATE
    struct editor_state {
        struct termios orig_termios;
        size_t rows;
        size_t cols;
        size_t cx;
        size_t cy;
    };
#endif

#ifndef ABUF
#define ABUF
#define ABUF_INIT {NULL, 0}

struct abuf {
    char *b;
    int len;
};
#endif

int get_window_size(struct editor_state *state);
void enable_raw_mode(struct editor_state *state);
int get_cursor_pos(size_t *rows, size_t *cols);
void disable_raw_mode();
void clean_exit(const char *msg);
void die(const char *msg);
void ab_append(struct abuf *ab, const char *s, int len);
void ab_free(struct abuf *ab);