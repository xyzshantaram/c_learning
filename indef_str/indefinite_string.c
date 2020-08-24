#include <stdlib.h>
#include <string.h>
#include "indefinite_string.h"

int round_up(int number, int base) {
    return number + (base - (number % base));
}

char* new_string(const char* input) {
    if (input != NULL) {
        size_t alloc_size = round_up(strlen(input), STRING_CHUNK_SIZE);
        char* ret = malloc(alloc_size);
        if (ret != NULL) {
            strcpy(ret, input);
        }
        else {
            ret = calloc(1, STRING_CHUNK_SIZE);
        }
        return ret;
    }
    else return NULL;
}

char* append_to_string(char* dest, const char* input) {
    size_t input_len = strlen(input);
    size_t cur_size = strlen(dest);
    size_t realloc_size = round_up(cur_size + input_len, STRING_CHUNK_SIZE);
    char* realloc_ret = realloc(dest, realloc_size);
    if (realloc_ret) {
        strcat(realloc_ret, input);
    }
    else {
        free(dest);
    }
    return realloc_ret;
}
