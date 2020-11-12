#ifndef __HIGHLIGHTING_H
#define __HIGHLIGHTING_H
#define HL_HIGHLIGHT_NUMBERS (1 << 0)
#define HL_HIGHLIGHT_STRINGS (1 << 1)

#include <stdlib.h>

#include "editor.h"
#include "structs.h"

struct editor_syntax {
    char *filetype;
    char **filematch;
    char **keywords;
    char *singleline_comment_start;
    char *multiline_comment_start;
    char *multiline_comment_end;
    int flags;
};

#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

enum editor_highlights
{
    HL_NORMAL = 0,
    HL_NUMBER,
    HL_MATCH,
    HL_STRING,
    HL_COMMENT,
    HL_KEYWORD1,
    HL_KEYWORD2,
    HL_MLCOMMENT,
};

void editor_update_syntax(struct editor_state *state, e_row *row);
int is_separator(int c);
#endif