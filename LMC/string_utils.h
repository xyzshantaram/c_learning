#pragma once

struct StrInfo {
    size_t len;
    size_t old_len;
    char* ptr;
};
#undef String
typedef struct StrInfo String;
#undef READ_CHUNK_SIZE
#define READ_CHUNK_SIZE 32

// Append a string to another without modifying either string. String.ptr will be NULL if realloc fails.
String lazy_append_to_string(char* dest, const char* input);

// Same as lazy_append, but allows you to track sizes manually. Useful for large strings.
String append_to_string(char* dest, const char* input, size_t dest_size, size_t input_len);

// Reads a string from stdin until stop_char is reached. Returns NULL if lazy_append fails.
char* read_string(char stop_char);

// Returns NULL if malloc'ing or realloc'ing fails
char** split_on_delim(char* str, size_t* index, const char DELIM);

// Returns NULL if realloc'ing fails.
char* tokenize_string(char* str, char DELIM, size_t* index);

// check if an array of strings contains an exact copy of a given string.
int str_array_contains(const char* arr[], size_t arr_len, char* str);

// check if an array of strings contains either an exact copy of a given string or
// a superset of the given string.
int str_array_fuzzy_contains(const char* arr[], size_t arr_len, char* str);

// Returns NULL on read failure.
char* file_into_str(const char* filename);