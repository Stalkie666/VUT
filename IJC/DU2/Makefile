# Makefile
#  // Řešení IJC-DU1, příklad a+b), 17.4.2023
#  // Autor: Jakub Hamadej, FIT
#  // Přeloženo: Je to Makefile, nepreklada se

CC=gcc
CFLAGS=-std=c11 -pedantic -Wall -Wextra -g
XLOGIN=xhamad03

default: tail wordcount wordcount-dynamic libhtab.so wordcount-

run: tail wordcount wordcount-dynamic libhtab.so wordcount-
	./tests/testIJC2.sh

tail: tail.c
	$(CC) $(CFLAGS) $^ -o $@

wordcount: wordcount.c io.o libhtab.a
	$(CC) $(CFLAGS) -static -o $@ $^

wordcount-dynamic: wordcount.c io.o libhtab.so
	$(CC) $(CFLAGS) -o $@ wordcount.c io.o -L. -lhtab 

wordcount-: wordcount-.cc
	g++ -Wall -pedantic -Wextra -o $@ $^

io.o: io.c io.h
	$(CC) $(CFLAGS) -c -o $@ $<

# static library

libhtab.a: htab_bucket_count.o htab_clear.o htab_erase.o htab_find.o htab_for_each.o htab_free_item.o htab_free.o htab_hash_function.o htab_init_item.o htab_init.o htab_lookup_add.o htab_size.o htab_statistics.o
	ar -rcs $@ $^
	ranlib $@

htab_bucket_count.o: htab_bucket_count.c htab_private.h
	$(CC) $(CFLAGS) -c $<
htab_clear.o: htab_clear.c htab_private.h
	$(CC) $(CFLAGS) -c $<
htab_erase.o: htab_erase.c htab_private.h
	$(CC) $(CFLAGS) -c $<
htab_find.o: htab_find.c htab_private.h
	$(CC) $(CFLAGS) -c $<
htab_for_each.o: htab_for_each.c htab_private.h
	$(CC) $(CFLAGS) -c $<
htab_free_item.o: htab_free_item.c htab_private.h
	$(CC) $(CFLAGS) -c $<
htab_free.o: htab_free.c htab_private.h
	$(CC) $(CFLAGS) -c $<
htab_hash_function.o: htab_hash_function.c htab_private.h
	$(CC) $(CFLAGS) -c $<
htab_init_item.o: htab_init_item.c htab_private.h
	$(CC) $(CFLAGS) -c $<
htab_init.o: htab_init.c htab_private.h
	$(CC) $(CFLAGS) -c $<
htab_lookup_add.o: htab_lookup_add.c htab_private.h
	$(CC) $(CFLAGS) -c $<
htab_size.o: htab_size.c htab_private.h
	$(CC) $(CFLAGS) -c $<
htab_statistics.o: htab_statistics.c htab_private.h
	$(CC) $(CFLAGS) -c $<





# dynamic library

libhtab.so: dyn-htab_bucket_count.o dyn-htab_clear.o dyn-htab_erase.o dyn-htab_find.o dyn-htab_for_each.o dyn-htab_free_item.o dyn-htab_free.o dyn-htab_hash_function.o dyn-htab_init_item.o dyn-htab_init.o dyn-htab_lookup_add.o dyn-htab_size.o dyn-htab_statistics.o
	$(CC) $(CFLAGS) -shared -fPIC $^ -o $@

dyn-htab_bucket_count.o: htab_bucket_count.c htab_private.h
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
dyn-htab_clear.o: htab_clear.c htab_private.h
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
dyn-htab_erase.o: htab_erase.c htab_private.h
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
dyn-htab_find.o: htab_find.c htab_private.h
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
dyn-htab_for_each.o: htab_for_each.c htab_private.h
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
dyn-htab_free_item.o: htab_free_item.c htab_private.h
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
dyn-htab_free.o: htab_free.c htab_private.h
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
dyn-htab_hash_function.o: htab_hash_function.c htab_private.h
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
dyn-htab_init_item.o: htab_init_item.c htab_private.h
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
dyn-htab_init.o: htab_init.c htab_private.h
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
dyn-htab_lookup_add.o: htab_lookup_add.c htab_private.h
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
dyn-htab_size.o: htab_size.c htab_private.h
	$(CC) $(CFLAGS) -c -fPIC $< -o $@
dyn-htab_statistics.o: htab_statistics.c htab_private.h
	$(CC) $(CFLAGS) -c -fPIC $< -o $@

zip:
	zip -r $(XLOGIN).zip *.c *.h *.cc Makefile tests/

clean:
	rm -f tail *.o libhtab.a libhtab.so wordcount wordcount- wordcount-dynamic $(XLOGIN).zip
