#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

#ifndef TUTILS_INCL
    #define TUTILS_INCL
    #include "termutils.h"
#endif

#define CTRL_KEY(k) ((k)&0x1f)

#ifndef E_ROW
    #define E_ROW
    typedef struct e_row {
        int size;
        char *chars;
    } e_row;
#endif

#ifndef ESTATE
    #define ESTATE
    struct editor_state {
        struct termios orig_termios;
        struct winsize ws;
        size_t rows;
        size_t cols;
        size_t cx;
        size_t cy;
        size_t rx;
        size_t n_rows;
        e_row row;
    };
#endif

#ifndef EDITOR_FNS
    #define EDITOR_FNS
    void editor_draw_rows(struct editor_state *state, struct abuf *ab);
    void editor_refresh_screen(struct editor_state *state);
    void init_editor(struct editor_state *state);
    void editor_move_cursor(struct editor_state *state, int key);
    int editor_read_key();
    void editor_process_keypress();
    void editor_open_file(struct editor_state *state, const char *filename);
#endif

#ifndef KEY_ENUM
#define KEY_ENUM
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
#endif