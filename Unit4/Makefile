CC=gcc
CFLAGS=-Wall -Wextra -g
SRC=$(wildcard *.c)
BIN=$(SRC:.c=)
all: $(BIN)
%: %.c
	$(CC) $(CFLAGS) $< -o $@ -lm
clean:
	rm -f $(BIN)
