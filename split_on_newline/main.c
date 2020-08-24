#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "indefinite_string.h"

#define DELIM "\n"

/*
   A reimplementation of strtok. Returns a pointer to a string which is a
   substring of the given string from *index to the first occurrence of delim.
   Arguments: char* str      --> the string we're gonna tokenise.
              char* delim    --> The delimiter string
              size_t* index  --> Pointer to an index variable at which to reenter the string.
   If index is equal to the string length, NULL is returned.
   Combined delimiters cause empty lines to be added to the array.
   Returned pointer should be freed as usual.
*/
char* tokenize_string(char* str, char* delim, size_t* index) {
    size_t str_len = strlen(str);
    size_t i = *index;
    if (i >= str_len) {
        return NULL;
    }
    char* ret = strdup("");
    // i already has a value; no need to reassign.
    for (; i < str_len; i += 1) {
        if(str[i] == *delim) {
            break;
        }
        char* new_ret = realloc(ret, strlen(ret) + sizeof(char));
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

char** split_on_newline(char* str, size_t* index) {
    size_t arr_size = 0;
    size_t split_index = 0; // index for reentry into the string for consecutive splits
    char* token = tokenize_string(str, DELIM, &split_index);
    char** arr = malloc(sizeof(char*));
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
    free(token);
    return arr;
}

int main() {
    int retval = 0;
    char* to_split = strdup("This\nis\na\nstring\nwith\ntons\nof\nnewlines");
    size_t index = 0;
    char **arr = split_on_newline(to_split, &index);
    if (arr) {
        for (size_t i = 0; i < index; i++) {
            printf("%s\n", arr[i]);
            free(arr[i]);
        }
    }
    else {
        printf("Tokenization failed due to reallocation error. Exiting...");
        retval = 1;
        goto ret;
    }
    free(to_split);
    free(arr);
    ret:
    return retval;
}
