#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#ifndef TUTILS_INCL
    #define TUTILS_INCL
    #include "termutils.h"
#endif

#define KILO_VERSION "ch 4"
#define TAB_SIZE 4
#define STATUSLINE_COUNT 2

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#define CTRL_KEY(k) ((k)&0x1f)

#ifndef E_ROW
    #define E_ROW
    typedef struct e_row {
        int size;
        char *chars;
        int render_size;
        char* render;
    } e_row;
#endif

#ifndef ESTATE
    #define ESTATE
    struct editor_state {
        struct termios orig_termios;
        size_t rows;
        size_t cols;
        size_t cx;
        size_t cy;
        size_t rx;
        size_t row_offset;
        size_t column_offset;
        size_t n_rows;
        e_row *row;
        char *filename;
        char status_msg[80];
        time_t status_time;
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
    void editor_append_row(struct editor_state *state, char *s, size_t len);
    void editor_update_row(e_row *row);
    void editor_set_status(struct editor_state *state, const char *fmt, ...);
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