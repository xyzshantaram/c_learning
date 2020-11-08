#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#ifndef ABUF
#define ABUF
#define ABUF_INIT \
    {             \
        NULL, 0   \
    }

struct abuf
{
    char *b;
    int len;
};
#endif

#ifndef EDITOR_INCL
    #define EDITOR_INCL
    #include "editor.h"
#endif

int get_window_size(struct editor_state *state);
void enable_raw_mode(struct editor_state *state);
int get_cursor_pos(size_t *rows, size_t *cols);
void disable_raw_mode(struct editor_state *state);
void clean_exit(const char *msg, struct editor_state *state);
void die(const char *msg, struct editor_state *state);
void ab_append(struct abuf *ab, const char *s, int len);
void ab_free(struct abuf *ab);