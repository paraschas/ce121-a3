CC = gcc
CFLAGS = -Wall -g

.PHONY: all

all: a3
	@echo "done"

a3:
	$(CC) $(CFLAGS) a3.c -o a3
