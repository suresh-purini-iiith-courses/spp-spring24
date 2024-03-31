# Compiler settings - Can be customized.
CC = gcc
CFLAGS = -fopenmp
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.out)

# Default target.
all: $(OBJ)

# Rule for building final object.
%.out: %.c
	$(CC) $(CFLAGS) $< -o $@

# Clean up
clean:
	rm -f $(OBJ)

# Prevent make from doing something with files named all or clean.
.PHONY: all clean
