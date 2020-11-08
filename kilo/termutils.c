#include "editor.h"
#include "termutils.h"

int get_window_size(struct editor_state *state) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
            return -1;
        return get_cursor_pos(&(state->rows), &(state->cols));
    } 
    else {
        state->cols = ws.ws_col;
        state->rows = ws.ws_row;
        return 0;
    }
}

int get_cursor_pos(size_t *rows, size_t *cols) {
    char buf[32];
    unsigned int i = 0;
    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }
    buf[i] = '\0';
    if (buf[0] != '\x1b' || buf[1] != '[')
        return -1;
    if (sscanf(&buf[2], "%zu;%zu", rows, cols) != 2)
        return -1;
    return 0;
}

void disable_raw_mode(struct editor_state *state) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &(state->orig_termios)) == -1)
        die("tcsetattr", state);
}

void enable_raw_mode(struct editor_state *state) {
    if (tcgetattr(STDIN_FILENO, &(state->orig_termios)) == -1)
        die("tcgetattr", state);
    struct termios raw = state->orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr", state);
}

void die(const char *msg, struct editor_state *state) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    perror(msg);
    disable_raw_mode(state);
    exit(1);
}

void clean_exit(const char *msg, struct editor_state *state) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    disable_raw_mode(state);
    printf(msg);
    exit(0);
}

void ab_append(struct abuf *ab, const char *s, int len) {
    char *new = realloc(ab->b, ab->len + len);
    if (new == NULL) return;
    memcpy(&new[ab->len], s, len);
    ab->b = new;
    ab->len += len;
}
void ab_free(struct abuf *ab) {
    free(ab->b);
}