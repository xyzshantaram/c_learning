#include "editor.h"
#include "termutils.h"

struct editor_state state;

int main(int argc, char* argv[]) {
    enable_raw_mode(&state);
    init_editor(&state);
    if (argc >= 2) {
        editor_open_file(&state, argv[1]);
    }
    while (1) {
        editor_refresh_screen(&state);
        editor_process_keypress(&state);
    }
    disable_raw_mode(&state);
    return 0;
}