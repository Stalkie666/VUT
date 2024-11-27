CPP=g++
CPPFLAGS= -lsimlib -Wall -pedantic

default: main

main: main.cpp
	$(CPP) $(CPPFLAGS) main.cpp -o main
run: main
	./main
clean:
	rm -rf main