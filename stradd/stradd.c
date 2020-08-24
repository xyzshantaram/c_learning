#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_STR_SIZE 128

char* concat_strings(char* str1, char* str2, bool strip_last_char) {
    size_t str1_len, str2_len;
    str1_len = strlen(str1);
    str2_len = strlen(str2);   
    size_t new_len = str1_len + str2_len + 1;
    char* concatenation = malloc(new_len);
    strncpy(concatenation, str1, str1_len);
    strncat(concatenation, str2, str2_len);

    return concatenation;
}

int main() {
    char *str1 = malloc(MAX_STR_SIZE);
    char *str2 = malloc(MAX_STR_SIZE);

    printf("Maximum allowed length is %i\n", MAX_STR_SIZE);

    printf("Enter a string: ");
    if (fgets(str1, MAX_STR_SIZE, stdin) == NULL) {
        goto error_cleanup;
    }
    
    printf("Enter another string: ");
    if (fgets(str2, MAX_STR_SIZE, stdin) == NULL) {
        goto error_cleanup;
    }
    
    printf("concatenated strings: %s\n", concat_strings(str1, str2, true));
    
    free(str1);
    free(str2);
    return 0;

    error_cleanup:
        free(str1);
        free(str2);
        return 1;
}
