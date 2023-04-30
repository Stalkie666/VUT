CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread

proj2 : proj2.c proj2.h
	$(CC) $(CFLAGS) proj2.c -o proj2

zip:
	zip proj2.zip proj2.c proj2.h Makefile

clean:
	rm -rf proj2 proj2.zip proj2.out