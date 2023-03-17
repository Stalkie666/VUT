# Řešení IJC-DU1, příklad a+b), 22.3.2023
# Autor: Jakub Hamadej, FIT
# Přeloženo: toto je Makefile

CC=gcc
CFLAGS=-g -std=c11 -pedantic -Wall -Wextra
LIBS=-lm
XLOGIN=xhamad03
TARGETS=primes primes-i steg-decode

run: $(TARGETS)
	./primes
	./primes-i

primes: primes.o eratosthenes.o error.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

primes-i: primes-i.o eratosthenes-i.o error.o bitset.o
	$(CC) $(CFLAGS) -DUSE_INLINE $^ $(LIBS) -o $@ 

steg-decode: steg-decode.o eratosthenes.o ppm.o error.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

primes.o: primes.c eratosthenes.h
	$(CC) $(CFLAGS) -c -o $@ primes.c
steg-decode.o: steg-decode.c bitset.h error.h ppm.h
	$(CC) $(CFLAGS) -c -o $@ steg-decode.c 
eratosthenes.o: eratosthenes.c eratosthenes.h bitset.h
	$(CC) $(CFLAGS) -c -o $@ eratosthenes.c
error.o: error.c error.h
	$(CC) $(CFLAGS) -c -o $@ error.c
bitset.o: bitset.c bitset.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c -o $@ bitset.c
ppm.o: ppm.c ppm.h error.h
	$(CC) $(CFLAGS) -c -o $@ ppm.c
primes-i.o: primes.c eratosthenes.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c -o $@ primes.c
eratosthenes-i.o: eratosthenes.c eratosthenes.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c -o $@ eratosthenes.c




zip:
	zip $(XLOGIN).zip *.c *.h Makefile

clean:
	rm -f $(TARGETS) *.o $(XLOGIN).zip