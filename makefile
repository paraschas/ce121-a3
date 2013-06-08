CC = gcc
CFLAGS = -Wall -g

.PHONY: all

all: scee integers times
	@echo "done"

scee: scee.c
	$(CC) $(CFLAGS) scee.c -o scee

integers: integers.c
	$(CC) $(CFLAGS) integers.c -o integers

times: times.c
	$(CC) $(CFLAGS) times.c -o times

clean:
	rm scee integers times
