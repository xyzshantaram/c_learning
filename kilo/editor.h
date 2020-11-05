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
};
#endif

void editor_draw_rows(struct editor_state *state);
void editor_refresh_screen(struct editor_state *state);
void init_editor(struct editor_state *state);

char editor_read_key();
void editor_process_keypress();