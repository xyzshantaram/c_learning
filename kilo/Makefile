NAME = kilo
CC ?= gcc # don't override CC if one is already set, or if the user has already set one
CFLAGS := -Wall -Wextra -Wunreachable-code -O0
OBJ = kilo.o termutils.o editor.o
HEADER = editor.h termutils.h
OUTPUT_NAME = kilo

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

test: $(NAME)
	./$(OUTPUT_NAME) editor.c

clean: 
	rm -f $(OBJ)
	rm -f $(OUTPUT_NAME)
