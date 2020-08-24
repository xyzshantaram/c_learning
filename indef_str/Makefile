NAME = main
CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wunreachable-code
DEPENDS = indefinite_string.o
DEPEND_HEADER_FILES = indefinite_string.h
OUTPUT_FILE = a.out

default: $(NAME)

# target: depend1.extension depend2.extension ... dependN.extension
#         command1
#         command2

$(NAME): $(NAME).o $(DEPENDS)
	  $(CC) $(CFLAGS) $(NAME).o $(DEPENDS) -o $(OUTPUT_FILE)

$(NAME).o: $(NAME).c $(DEPEND_HEADER_FILES) 
	  $(CC) -c $(NAME).c

indefinite_string.o: indefinite_string.h indefinite_string.c
	  $(CC) $(CFLAGS) -c indefinite_string.c

run: $(NAME)
	  ./$(OUTPUT_FILE)
