NAME = main
CC ?= gcc # don't override CC if one is already set, or if the user has already set one
CFLAGS := -Wall -Wextra -Wunreachable-code
OBJ = main.o string_utils.o lmc.o
HEADER = string_utils.h lmc.h
OUTPUT_NAME = a.out

$(NAME): $(OBJ)
	  $(CC) -o $(OUTPUT_NAME) $(CFLAGS) $(OBJ)

debug: CFLAGS += -g -ggdb
debug: $(NAME)

debug_mem: CFLAGS += -g -ggdb -fsanitize=address
debug_mem: $(NAME)

main.c: $(HEADERS)
string_utils.c: $(HEADERS)

run: $(NAME)
	  ./$(OUTPUT_NAME)

clean: 
	rm $(OBJ)
	rm $(OUTPUT_NAME)
