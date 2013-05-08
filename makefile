CC = gcc
CFLAGS = -Wall -g

.PHONY: all

all: a3 seconds times
	@echo "done"

a3:
	$(CC) $(CFLAGS) a3.c -o a3

seconds:
	$(CC) $(CFLAGS) seconds.c -o seconds

times:
	$(CC) $(CFLAGS) times.c -o times
