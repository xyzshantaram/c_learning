#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "indefinite_string.h"
#include <string.h>
#define READ_CHUNK_SIZE 32
int main() {
    int retval = 0;
    FILE* file_ptr;
    char* file_contents = strdup("");
    file_ptr = fopen("main.c", "r");
    if(file_ptr == NULL) {
        printf("fopen failed\n");
        return 1;
    }

    char* temp = malloc(READ_CHUNK_SIZE);

    while (!feof(file_ptr)) {
        temp = fgets(temp, READ_CHUNK_SIZE, file_ptr);
        if (temp != NULL) {
            file_contents = append_to_string(file_contents, (const char*) temp);
        }
        else {
            if (errno != 0) {
                //TODO: implement specific error handling
                retval = errno;
                goto cleanup;
            }
        }
    }
    printf("%s", file_contents);
    goto cleanup;
    
    cleanup:
        fclose(file_ptr);
        free(temp);
        free(file_contents);
        return retval;
}
