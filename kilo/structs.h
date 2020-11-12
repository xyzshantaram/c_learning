#ifndef __STRUCTS_H
#define __STRUCTS_H

#include <stdio.h>
#include <time.h>
#include <termios.h>

typedef struct e_row {
    int idx;
    size_t size;
    char *chars;
    size_t render_size;
    char *render;
    unsigned char *hl;
    int hl_open_comment;
} e_row;

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
    struct editor_syntax *syntax;
};
#endif