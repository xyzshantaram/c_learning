#include <stdlib.h>
#include <string.h>
#include "indefinite_string.h"

char* append_to_string(char* dest, const char* input) {
    size_t input_len = strlen(input);
    size_t cur_size = strlen(dest);
    size_t realloc_size = cur_size + input_len;
    char* realloc_ret = realloc(dest, realloc_size);
    if (realloc_ret) {
        strcat(realloc_ret, input);
    }
    else {
        free(dest);
    }
    return realloc_ret;
}
