CPP=g++
CPPFLAGS=-Wall -pedantic

default: main.cpp isa-top.cpp isa-top.hpp
	$(CPP) $(CPPFLAGS) main.cpp isa-top.cpp -o isa-top -lncurses

clean:
	rm -rf isa-top