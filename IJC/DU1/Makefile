CC=gcc
CFLAGS=-g -std=c11 -pedantic -Wall -Wextra
LIBS=-lm
XLOGIN=xhamad03

run: primes primes-i

primes: primes.c eratosthenes.c error.c
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

primes-i: primes.c eratosthenes.c error.c bitset.c
	$(CC) $(CFLAGS) -DUSE_INLINE $^ $(LIBS) -o $@ 

steg-decode: steg-decode.c eratosthenes.c ppm.c error.c
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


zip:
	zip $(XLOGIN).zip *.c *.h Makefile

clean:
	rm -f *.o primes primes-i steg-decode