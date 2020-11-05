#include "editor.h"
#include "termutils.h"

void editor_draw_rows(struct editor_state *state)
{
    for (size_t y = 0; y < state->rows; y++)
    {
        write(STDOUT_FILENO, "~\r\n", 3);
    }
}
void editor_refresh_screen(struct editor_state* state)
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    editor_draw_rows(state);
    write(STDOUT_FILENO, "\x1b[H", 3);
}

char editor_read_key() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN)
            die("read");
    }
    return c;
}

void init_editor(struct editor_state* state) {
    if (get_window_size(state) == -1)
        die("get_window_size");
}

void editor_process_keypress() {
    char c = editor_read_key();
    switch (c) {
    case CTRL_KEY('q'):
        clean_exit("Exit called by user\r\n");
        break;
    }
}