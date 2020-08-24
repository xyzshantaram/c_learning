#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "string_utils.h"


String lazy_append_to_string(char* dest, const char* input) {
    size_t input_len = strlen(input);
    size_t cur_size = strlen(dest);
    size_t realloc_size = cur_size + input_len + 1;
    char* realloc_ret = realloc(dest, realloc_size);
    String ret = { .len = realloc_size, .ptr = NULL, .old_len = cur_size };
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

String append_to_string(char* dest, const char* input, size_t dest_size, size_t input_len) {
    size_t realloc_size = dest_size + input_len + 1;
    char* realloc_ret = realloc(dest, realloc_size);
    if (realloc_ret) {
        strcat(realloc_ret, input);
    }
    else {
        free(dest);
    }
    String ret = { .len = realloc_size, .ptr = realloc_ret, .old_len = dest_size };
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

char* tokenize_string(char* str, char DELIM, size_t* index) {
    size_t str_len = strlen(str);
    size_t i = *index;
    if (i >= str_len) {
        return NULL;
    }
    char* ret = strdup("");
    // i already has a value; no need to reassign.
    for (; i < str_len; i += 1) {
        if(str[i] == DELIM) {
            break;
        }
        char* new_ret = realloc(ret, strlen(ret) + sizeof(char) + 1);
        if (new_ret) {
            ret = new_ret;
        }
        else {
            free(ret);
            return NULL;
        }
        strncat(ret, &str[i], 1);
    }
    *index = i + 1; // +1 because we break at the delimiter, and we need to move to the next index after that
    return ret;
}

char** split_on_delim(char* str, size_t* index, const char DELIM) {
    size_t arr_size = 0;
    size_t split_index = 0; // index for reentry into the string for consecutive splits
    char* token = tokenize_string(str, DELIM, &split_index);
    char** arr = malloc(sizeof(char*));
    if (arr) {
        while(token) {
            arr_size += sizeof(char*);
            char** new_arr = realloc(arr, arr_size);
            if (new_arr) {
                arr = new_arr;
                arr[*index] = token;
            }
            else {
                free(arr);
                return NULL;
            }
            *index += 1;
            token = tokenize_string(str, DELIM, &split_index);
        }
    }
    else {
        return NULL;
    }
    free(token);
    return arr;
}

int str_array_contains(const char* arr[], size_t arr_len, char* str) {
    for (size_t i = 0; i < arr_len; i++) {
        if (strcmp(arr[i], str) == 0) return i;
    }
    return -1;
}

int str_array_fuzzy_contains(const char* arr[], size_t arr_len, char* str) {
    for (size_t i = 0; i < arr_len; i++) {
        if (strstr(str, arr[i])) return i;
    }
    return -1;
}

char* file_into_str(const char* filename) {
    FILE* file_ptr;
    char* file_contents = strdup("");
    String new_str;

    file_ptr = fopen(filename, "r");
    if(file_ptr == NULL) {
        return NULL;
    }

    char buffer[READ_CHUNK_SIZE];

    while (!feof(file_ptr)) {
        fgets(buffer, READ_CHUNK_SIZE - 1, file_ptr);
        new_str = lazy_append_to_string(file_contents, buffer);
        if (new_str.ptr) {
            file_contents = new_str.ptr;
        }
        else {
            free(new_str.ptr);
            free(file_contents);
            file_contents = NULL;
            break;
        }
    }
    fclose(file_ptr);
    return file_contents;
}