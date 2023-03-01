CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g -std=c11

default: run

run: primes primes-i steg-decode

primes: primes.o
	$(CC) $(CFLAGS) -o $@ $^
primes-i: primes-i.o
	$(CC) $(CFLAGS) -o $@ $^
steg-decode: steg-decode.o
	$(CC) $(CFLAGS) -o $@ $^

steg-decode.o: steg-decode.c
	$(CC) $(CFLAGS) -o $@ $^
primes.o: primes.c bitset.h
	$(CC) $(CFLAGS) -o $@ $^
primes-i.o: primes.c bitset.h
	$(CC) $(CFLAGS) -DUSE_INLINE -o $@ $^

clean:
	rm -f *.o primes primes-i steg-decode