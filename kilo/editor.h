#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "termutils.h"

#define CTRL_KEY(k) ((k)&0x1f)

#ifndef ESTATE
    #define ESTATE
    struct editor_state
    {
        struct termios orig_termios;
        struct winsize ws;
        size_t rows;
        size_t cols;
        size_t cx;
        size_t cy;
    };
#endif

void editor_draw_rows(struct editor_state *state, struct abuf *ab);
void editor_refresh_screen(struct editor_state *state);
void init_editor(struct editor_state *state);
void editor_move_cursor(struct editor_state *state, int key);
int editor_read_key();
void editor_process_keypress();

enum editor_key {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    PAGE_UP,
    PAGE_DOWN,
    HOME,
    END,
    DEL
};

typedef struct erow {
  int size;
  char *chars;
} erow;
