CC = gcc
CFLAGS = -Wall -g

.PHONY: all

all: a3 integers times
	@echo "done"

a3:
	$(CC) $(CFLAGS) a3.c -o a3

integers:
	$(CC) $(CFLAGS) integers.c -o integers

times:
	$(CC) $(CFLAGS) times.c -o times
