#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define READ_CHUNK_SIZE 32
#pragma once

struct StrInfo {
    size_t len;
    size_t old_len;
    char* ptr;
};
#undef String
typedef struct StrInfo String;

String lazy_append_to_string(char* dest, const char* input) {
    size_t input_len = strlen(input);
    size_t cur_size = strlen(dest);
    size_t realloc_size = cur_size + input_len + 1;
    char* realloc_ret = (char*) realloc(dest, realloc_size);
    String ret = { .len = realloc_size, .old_len = cur_size, .ptr = NULL };
    if (realloc_ret) {
        strcat(realloc_ret, input);
    }
    else {
        free(dest);
    }
    ret.len = realloc_size;
    ret.ptr = realloc_ret; 
    ret.old_len = cur_size;
    return ret;
}

char* read_string(char stop_char) {
    char* final = strdup("");
    String new_str;
    char buffer[READ_CHUNK_SIZE];
    while (fgets(buffer, sizeof(buffer), stdin) != 0) {
        new_str = lazy_append_to_string(final, buffer);
        if (new_str.ptr) {
            final = new_str.ptr;
        }
        else {
            free(new_str.ptr);
            free(final);
            return NULL;
        }
        if (final[new_str.len - 2] == stop_char) break;
    }
    final[new_str.len - 1] = '\0';
    return final;
}


long int pow_accum(long int a, long int m, long int r){
    if (m == 0) return r;
    else  return pow_accum(a, m - 1 , a * r);
}

long int pow(long int a, long int m) {
    return pow_accum(a, m, 1);
}

int main() {
    long n = strtol(read_string('\n'), NULL, 10);
    long m = strtol(read_string('\n'), NULL, 10);
    size_t printed = 0;

    if (errno != 0) {
        printf("Long conversion of input failed, exiting");
        return 1;
    }
    
    for(size_t i = 2; i <= m; i++) {
        if (((int) pow(10, n) + 1) % i == 0) {
            printf("%i\n", i);
            printed++;
        }
    }
    
    if (!printed) {
        printf("No complete factors");
    }
    
    return 0;
}