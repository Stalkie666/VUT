CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic

default: proj2

proj2: main.o loadParametrs.o
	$(CC) $(CFLAGS) $^ -o $@

main.o:main.c loadParametrs.h
	$(CC) $(CFLAGS) main.c -c -o $@ 
loadParametrs.o:loadParametrs.c loadParametrs.h
	$(CC) $(CFLAGS) loadParametrs.c -c -o $@
clean:
	rm -rf proj2 *.o 