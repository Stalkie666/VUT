CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g -std=c11 -lm
XLOGIN=xhamad03

default: primes primes-i steg-decode

run: primes primes-i steg-decode
	./primes
	./primes-i

primes: primes.o
	$(CC) $(CFLAGS) -o $@ $^
primes-i: primes-i.o
	$(CC) $(CFLAGS) -DUSE_INLINE -o $@ $^
steg-decode: steg-decode.o
	$(CC) $(CFLAGS) -o $@ $^

steg-decode.o: steg-decode.c
	$(CC) $(CFLAGS) -o $@ $^
primes.o: primes.c bitset.h
	$(CC) $(CFLAGS) -o $@ $^
primes-i.o: primes.c bitset.h
	$(CC) $(CFLAGS) -DUSE_INLINE -o $@ $^

zip:
	zip $(XLOGIN).zip *.c *.h Makefile

clean:
	rm -f *.o primes primes-i steg-decode