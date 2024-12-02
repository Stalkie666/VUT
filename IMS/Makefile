CPP=g++
CPPFLAGS= -lsimlib -Wall -pedantic

default: main

main: main.cpp convoySettings.hpp
	$(CPP) $(CPPFLAGS) main.cpp -o main
run: main
	./main
pack:
	zip xhamad03.zip Makefile main.cpp convoySettings.hpp simulacniStudie.pdf

clean:
	rm -rf main xhamad03.zip