#include <stdio.h>
#include "indefinite_string.h"

#define STRING_CHUNK_SIZE 32

int main() {
    char* str = new_string("");
    char* str2 = new_string("str2");
    printf("%s", str);
    str = append_to_string(str, "this is a really really really really really really long string\n");
    str = append_to_string(str, "some other thing\n");
    str = append_to_string(str, str2);
    if (str == NULL) {
        printf("Append to str failed.");
        return 1;
    }
    printf("%s", str);
    return 0;
}
