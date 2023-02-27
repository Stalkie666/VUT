CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g -std=c11

default: run

run: primes primes-i steg-decode

primes: primes.o

primes-i: primes-i.o

steg-decode: steg-decode.o

steg-decode.o: steg-decode.c

primes.o: primes.c

primes-i.o: primes.c

clean:
	rm -f *.o primes primes-i