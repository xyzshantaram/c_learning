#include "editor.h"
#include "termutils.h"

#define KILO_VERSION "ch 3"

void editor_draw_rows(struct editor_state *state, struct abuf *ab) {
    for (size_t y = 0; y < state->rows; y++) {
        if (y == state->rows - 1) {
            char welcome_msg[96];
            size_t welcome_len = snprintf(welcome_msg, sizeof(welcome_msg), "kilo editor -- %s", KILO_VERSION);
            if (welcome_len > state->cols) welcome_len = state->cols;
            int padding = (state->cols - welcome_len) / 2;
            if (padding) {
                ab_append(ab, "~", 1);
                padding--;
            }
            while (padding--) ab_append(ab, " ", 1);
            ab_append(ab, welcome_msg, welcome_len);
        }
        else ab_append(ab, "~", 1);
        ab_append(ab, "\x1b[K", 3);
        if (y < state->rows - 1) {
            ab_append(ab, "\r\n", 2);
        }
    }
}
void editor_refresh_screen(struct editor_state* state)
{
    struct abuf ab = ABUF_INIT;
    ab_append(&ab, "\x1b[?25l", 6); // hide cursor
    editor_draw_rows(state, &ab);
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%zu;%zuH", state->cy + 1, state->cx + 1); // move cursor
    ab_append(&ab, buf, strlen(buf));
    ab_append(&ab, "\x1b[?25h", 6); // show cursor
    write(STDOUT_FILENO, ab.b, ab.len);
    ab_free(&ab);
}

int editor_read_key() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
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
                    case '7': return END;
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

void init_editor(struct editor_state *state) {
    state->cx = 0;
    state->cy = 0;
    if (get_window_size(state) == -1)
        die("get_window_size");
}

void editor_process_keypress(struct editor_state *state) {
    int c = editor_read_key();
    switch (c) {
    case CTRL_KEY('q'):
        clean_exit("Exit called by user\r\n");
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