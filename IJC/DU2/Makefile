CC=gcc
CFLAGS=-Wall -pedantic

default: tail

tail: tail.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f tail *.o
