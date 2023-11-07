CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm
FILES_REC=exa.c ../rec/btree.c ../btree.c ../test_util.c ../test.c
FILES_ITER=exa.c ../iter/btree.c ../iter/stack.c ../btree.c ../test_util.c ../test.c

.PHONY: test clean

test: $(FILES_REC)
	$(CC) -DEXA=1 $(CFLAGS) -o $@_rec $(FILES_REC)
	$(CC) -DEXA=1 $(CFLAGS) -o $@_iter $(FILES_ITER)

diff: test
	./test_rec > rec.txt
	./test_iter > iter.txt
	diff -u --report-identical-files --text rec.txt iter.txt
	rm rec.txt iter.txt

clean:
	rm -f test_rec
	rm -f test_iter
