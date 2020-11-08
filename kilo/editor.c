#include "editor.h"
#include "termutils.h"

void init_editor(struct editor_state *state) {
    state->cx = 0;
    state->cy = 0;
    state->n_rows = 0;
    state->row_offset = 0;
    state->column_offset = 0;
    state->rx = 0;
    state->row = NULL;
    state->filename = NULL;
    state->status_msg[0] = '\0';
    state->status_time = 0;
    if (get_window_size(state) == -1)
        die("get_window_size", state);
    state->rows -= STATUSLINE_COUNT;
}

void editor_open_file(struct editor_state *state, const char *filename) {
    free(state->filename);
    state->filename = strdup(filename);
    FILE *fp = fopen(filename, "r");
    if (!fp)
        die("fopen", state);

    char *line = NULL;
    size_t line_cap = 0;
    ssize_t line_len;
    while ((line_len = getline(&line, &line_cap, fp)) != -1) {
        while (line_len > 0 && (line[line_len - 1] == '\n' || line[line_len - 1] == '\r'))
            line_len--;
        editor_append_row(state, line, line_len);
    }
    free(line);
    fclose(fp);
}

void e_draw_message_bar(struct editor_state *state, struct abuf *ab) {
    ab_append(ab, "\x1b[K", 3);
    int msglen = strlen(state->status_msg);
    if (msglen > state->cols)
        msglen = state->cols;
    if (msglen && time(NULL) - state->status_time < 5)
        ab_append(ab, state->status_msg, msglen);
}

void editor_append_row(struct editor_state *state, char *s, size_t len) {
    state->row = realloc(state->row, sizeof(e_row) * (state->n_rows + 1));
    int at = state->n_rows;
    state->row[at].size = len;
    state->row[at].chars = malloc(len + 1);
    memcpy(state->row[at].chars, s, len);
    state->row[at].chars[len] = '\0';
    state->row[at].render_size = 0;
    state->row[at].render = NULL;
    editor_update_row(&state->row[at]);
    state->n_rows++;
}

void editor_draw_status(struct editor_state *state, struct abuf *ab) {
    ab_append(ab, "\x1b[7m", 4);
    int len = 0;
    char status[80], rstatus[80];
    len = snprintf(status, sizeof(status), "%.20s - %zu lines",
                       state->filename ? state->filename : "[No Name]", state->n_rows);
    int rlen = snprintf(rstatus, sizeof(rstatus), "Ln %zu: Col %zu", state->cy + 1, state->cx + 1);
    if (len > state->cols)
        len = state->cols;
    ab_append(ab, status, len);
    while (len < state->cols) {
        if (state->cols - len == rlen) {
            ab_append(ab, rstatus, rlen);
            break;
        }
        else {
            ab_append(ab, " ", 1);
            len++;
        }
    }
    ab_append(ab, "\x1b[m", 3);
    ab_append(ab, "\r\n", 2);
}

void editor_set_status(struct editor_state *state, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(state->status_msg, sizeof(state->status_msg), fmt, ap);
    va_end(ap);
    state->status_time = time(NULL);
}

void editor_draw_rows(struct editor_state *state, struct abuf *ab) {
    for (size_t y = 0; y < state->rows; y++) {
        size_t file_row = y +state->row_offset;
        if (file_row >= state->n_rows) {
            ab_append(ab, "~", 1);
        }
        else {
            int len = state->row[file_row].render_size - state->column_offset;
            if (len < 0) len = 0;
            if (len > (int) state->cols) len = state->cols;
            ab_append(ab, &state->row[file_row].render[state->column_offset], len);
        }
        ab_append(ab, "\x1b[K", 3);
        if (y < state->rows) {
            ab_append(ab, "\r\n", 2);
        }
    }
}

void editor_scroll(struct editor_state *state) {
    state->rx = state->cx;
    state->rx = 0;
    if (state->cy < state->n_rows) {
        state->rx = e_row_cx_to_rx(&state->row[state->cy], state->cx);
    }
    if (state->cy < state->row_offset) {
        state->row_offset = state->cy;
    }
    if (state->cy >= state->row_offset + state->rows) {
        state->row_offset = state->cy - state->rows + 1;
    }
    if (state->rx < state->column_offset) {
        state->column_offset = state->rx;
    }
    if (state->rx >= state->column_offset + state->cols) {
        state->column_offset = state->rx - state->cols + 1;
    }
}

void editor_refresh_screen(struct editor_state* state) {
    editor_scroll(state);

    struct abuf ab = ABUF_INIT;
    ab_append(&ab, "\x1b[?25l", 6); // hide cursor
    ab_append(&ab, "\x1b[H", 3);
    editor_draw_rows(state, &ab);
    editor_draw_status(state, &ab);
    e_draw_message_bar(state, &ab);
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%zu;%zuH", (state->cy - state->row_offset) + 1, state->rx + 1);
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
    case PAGE_DOWN: {
            if (c == PAGE_UP) {
                state->cy = state->row_offset;
            }
            else if (c == PAGE_DOWN) {
                state->cy = state->row_offset + state->rows - 1;
                if (state->cy > state->n_rows)
                    state->cy = state->n_rows;
            }
            int times = state->rows;
            while (times--)
                editor_move_cursor(state, c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
        }
    break;
    case HOME:
        state->cx = 0;
        break;
    case END:
        if (state->cy < state->n_rows)
            state->cx = state->row[state->cy].size;
        break;
    }
}

void editor_move_cursor(struct editor_state *state, int key) {
    e_row *row = (state->cy >= state->n_rows) ? NULL : &state->row[state->cy];
    switch (key) {
    case ARROW_LEFT:
        if (state->cx != 0) {
            state->cx--;
        }
        else if (state->cx == 0) {
            if (state->cy > 0) {
                state->cy--;
                state->cx = state->row[state->cy].size;
            }
        }
        break;
    case ARROW_RIGHT:
        if (row && state->cx < row->size) {
            state->cx++;
        }
        if (state->cx == row->size) {
            if (state->cy < state->n_rows - 1) {
                state->cy++;
                state->cx = 0;
            }
        }
        break;
    case ARROW_UP:
        if (state->cy != 0) state->cy--;
        break;
    case ARROW_DOWN:
        if (state->cy < state->n_rows - 1) state->cy++;
        break;
    }
    row = (state->cy >= state->n_rows) ? NULL : &state->row[state->cy];
    int row_len = row ? row->size : 0;
    if (state->cx > row_len)
    {
        state->cx = row_len;
    }
}

void editor_update_row(e_row *row) {
    int tabs = 0;
    for (size_t j = 0; j <= row->size; j++) {
        if (row->chars[j] == '\t') tabs++;
    }
    free(row->render);
    row->render = malloc(row->size + (tabs * (TAB_SIZE - 1)) + 1);

    int idx = 0;
    for (size_t j = 0; j < row->size; j++) {
        if (row->chars[j] == '\t') {
            row->render[idx++] = ' ';
            while (idx % TAB_SIZE != 0) row->render[idx++] = ' ';
        }
        else {
            row->render[idx++] = row->chars[j];
        }
    }
    row->render[idx] = '\0';
    row->render_size = idx;
}

int e_row_cx_to_rx(e_row *row, int cx) {
    int rx = 0;
    int j;
    for (j = 0; j < cx; j++) {
        if (row->chars[j] == '\t')
            rx += (TAB_SIZE - 1) - (rx % TAB_SIZE);
        rx++;
    }
    return rx;
}