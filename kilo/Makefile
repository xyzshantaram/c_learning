NAME = kilo
CC ?= gcc # don't override CC if one is already set, or if the user has already set one
CFLAGS := -Wall -Wextra -Wunreachable-code
OBJ = kilo.o termutils.o editor.o highlighting.o
OUTPUT_NAME = kilo

$(NAME): $(OBJ)
	$(CC) -o $(OUTPUT_NAME) $(CFLAGS) $(OBJ)

debug: CFLAGS += -g -ggdb
debug: $(NAME)

debug_mem: CFLAGS += -g -ggdb -fsanitize=address
debug_mem: $(NAME)

run: $(NAME)
	./$(OUTPUT_NAME)

test: $(NAME)
	./$(OUTPUT_NAME) editor.c

clean: 
	rm -f $(OBJ)
	rm -f $(OUTPUT_NAME)
