#include "editor.h"
#include "termutils.h"

int get_window_size(struct editor_state *state) {
    struct winsize ws;
    if (1 || ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
            return -1;
        editor_read_key();
        return -1;
    }
    else {
        state->cols = ws.ws_col;
        state->rows = ws.ws_row;
        return 0;
    }
}

void disable_raw_mode(struct editor_state *state) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &(state->orig_termios)) == -1)
        die("tcsetattr");
}

void enable_raw_mode(struct editor_state *state) {
    if (tcgetattr(STDIN_FILENO, &(state->orig_termios)) == -1)
        die("tcgetattr");
    struct termios raw = state->orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

void die(const char *msg) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    perror(msg);
    exit(1);
}

void clean_exit(const char *msg) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    printf(msg);
    exit(0);
}