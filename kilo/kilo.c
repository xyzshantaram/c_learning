#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "editor.h"
#include "termutils.h"

struct editor_state state;

int main() {
    enable_raw_mode(&state);
    init_editor(&state);
    while (1) {
        editor_refresh_screen(&state);
        editor_process_keypress(&state);
    }
    disable_raw_mode(&state);
    return 0;
}