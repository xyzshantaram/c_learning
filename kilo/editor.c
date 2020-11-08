#include "editor.h"
#include "termutils.h"

#define KILO_VERSION "ch 3"

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#ifndef ESTATE
#define ESTATE
struct editor_state
{
    struct termios orig_termios;
    size_t rows;
    size_t cols;
    size_t cx;
    size_t cy;
    size_t n_rows;
    e_row *row;
};
#endif

void init_editor(struct editor_state *state) {
    state->cx = 0;
    state->cy = 0;
    state->n_rows = 0;
    state->row = NULL;
    if (get_window_size(state) == -1)
        die("get_window_size", state);
}

void editor_open_file(struct editor_state *state, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp)
        die("fopen", state);

    char *line = NULL;
    size_t line_cap = 0;
    ssize_t line_len;
    line_len = getline(&line, &line_cap, fp);
    if (line_len != -1)
    {
        while (line_len > 0 && (line[line_len - 1] == '\n' || line[line_len - 1] == '\r'))
            line_len--;
        state->row.size = line_len;
        state->row.chars = malloc(line_len + 1);
        memcpy(state->row.chars, line, line_len);
        state->row.chars[line_len] = '\0';
        state->n_rows = 1;
    }
    free(line);
    fclose(fp);
}

void editor_statusline(struct editor_state *state, struct abuf **ab, const char msg[]) {
    size_t msg_len = strlen(msg);
    if (msg_len > state->cols) msg_len = state->cols;
    int padding = (state->cols - msg_len) / 2;
    while (padding--)
        ab_append(*ab, " ", 1);
    char buf[msg_len + 1];
    memset(buf, 0, (msg_len + 1) * sizeof(char));
    strcpy(buf, msg);
    ab_append(*ab, buf, sizeof(buf));
}

void editor_draw_rows(struct editor_state *state, struct abuf *ab) {
    for (size_t y = 0; y < state->rows - 1; y++) {
        if (y >= state->n_rows && y != state->rows - 1) {
            ab_append(ab, "~", 1);
        }
        else {
            size_t len = state->row.size;
            if (len > state->cols) len = state->cols;
            ab_append(ab, state->row.chars, len);
        }
        ab_append(ab, "\x1b[K", 3);
        if (y < state->rows - 1) {
            ab_append(ab, "\r\n", 2);
        }
    }
    editor_statusline(state, &ab, "kilo editor -- " KILO_VERSION);
}
void editor_refresh_screen(struct editor_state* state) {
    struct abuf ab = ABUF_INIT;
    ab_append(&ab, "\x1b[?25l", 6); // hide cursor
    ab_append(&ab, "\x1b[H", 3);
    editor_draw_rows(state, &ab);
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%zu;%zuH", state->cy + 1, state->cx + 1); // move cursor
    ab_append(&ab, buf, strlen(buf));
    ab_append(&ab, "\x1b[?25h", 6); // show cursor
    write(STDOUT_FILENO, ab.b, ab.len);
    ab_free(&ab);
}

int editor_read_key(struct editor_state *state) {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read", state);
  }
  if (c == '\x1b') {
    char seq[3];
    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';
    if (seq[0] == '[') {
        if (seq[1] >= '0' && seq[1] <= '9') {
            if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
            if (seq[2] == '~') {
                switch (seq[1]) {
                    case '5': return PAGE_UP;
                    case '3': return DEL;
                    case '6': return PAGE_DOWN;
                    case '1': return HOME;
                    case '4': return END;
                    case '7': return HOME;
                    case '8': return END;
                }
        }
    } else {
      switch (seq[1]) {
        case 'A': return ARROW_UP;
        case 'B': return ARROW_DOWN;
        case 'C': return ARROW_RIGHT;
        case 'D': return ARROW_LEFT;
        case 'H': return HOME;
        case 'F': return END;
      }
    }    } else if (seq[0] == 'O') {
      switch (seq[1]) {
        case 'H': return HOME;
        case 'F': return END;
      }
  }
    return '\x1b';
  } else {
    return c;
  }
}

void editor_process_keypress(struct editor_state *state) {
    int c = editor_read_key(state);
    switch (c) {
    case CTRL_KEY('q'):
        clean_exit("Exit called by user\r\n", state);
        break;
    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
        editor_move_cursor(state, c);
        break;
    case PAGE_UP:
    case PAGE_DOWN:
        {
            int times = state->rows;
            while (times--)
                editor_move_cursor(state, c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
        }
    break;
    case HOME:
        state->cx = 0;
        break;
    case END:
        state->cx = state->cols - 1;
        break;
    }
}

void editor_move_cursor(struct editor_state *state, int key) {
    switch (key) {
    case ARROW_LEFT:
        if (state->cx != 0) state->cx--;
        break;
    case ARROW_RIGHT:
        if (state->cx != state->cols - 1) state->cx++;
        break;
    case ARROW_UP:
        if (state->cy != 0) state->cy--;
        break;
    case ARROW_DOWN:
        if (state->cy != state->rows - 1) state->cy++;
        break;
    }
}