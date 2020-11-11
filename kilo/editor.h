#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
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

#define KILO_VERSION "ch 6"
#define TAB_SIZE 4
#define STATUSLINE_COUNT 2
#define STATUS_TIMEOUT 2
#define QUIT_CONFIRM_COUNT 3

#define CTRL_KEY(k) ((k)&0x1f)

#ifndef E_ROW
    #define E_ROW
    typedef struct e_row {
        size_t size;
        char *chars;
        size_t render_size;
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
        int dirty;
        int last_key;
    };
#endif

#ifndef EDITOR_FNS
    #define EDITOR_FNS
    void editor_draw_rows(struct editor_state *state, struct abuf *ab);
    void editor_refresh_screen(struct editor_state *state);
    void init_editor(struct editor_state *state);
    void editor_move_cursor(struct editor_state *state, int key);
    int editor_read_key(struct editor_state *state);
    void editor_process_keypress(struct editor_state *state);
    void editor_open_file(struct editor_state *state, const char *filename);
    void editor_insert_row(struct editor_state *state, char *s, size_t len, size_t at);
    void editor_update_row(e_row *row);
    void editor_set_status(struct editor_state *state, const char *fmt, ...);
    char *editor_rows_to_string(struct editor_state *state, int *buf_len);
    void editor_row_append_string(struct editor_state *state, e_row *row, char *s, size_t len);
    void editor_delete_row(struct editor_state *state, size_t at);
    char *e_get_prompt_response(struct editor_state *state, const char *prompt, void (*callback)(struct editor_state *, char *, int));
    void editor_delete_char(struct editor_state *state);
    int e_row_cx_to_rx(e_row *row, int cx);
    void editor_insert_char(struct editor_state *state, int c);
    void editor_insert_newline(struct editor_state *state);
    void editor_find_callback(struct editor_state *state, char *query, int key);
    void editor_find(struct editor_state *state);
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
            DEL,
            BACKSPACE = 127,
        };
#endif