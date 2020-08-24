NAME = main
CC ?= gcc # don't override CC if one is already set, or if the user has already set one
CFLAGS := -Wall -Wextra -Werror -Wunreachable-code
OBJ = main.o indefinite_string.o
HEADER = indefinite_string.h
OUTPUT_NAME = a.out

$(NAME): $(OBJ)
	  $(CC) -o $(OUTPUT_NAME) $(CFLAGS) $(OBJ)

main.c: $(HEADERS)
indefinite_string.c: $(HEADERS)

run: $(NAME)
	  ./$(OUTPUT_NAME)

