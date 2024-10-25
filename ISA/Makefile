CPP=g++
CPPFLAGS=-Wall -pedantic -lncurses -lpcap

default: isa-top

isa-top: isa-top.o packet_handler.o main.o
	$(CPP) $(CPPFLAGS) isa-top.o packet_handler.o main.o -o isa-top

main.o: main.cpp isa-top.hpp packet_handler.hpp
	$(CPP) $(CPPFLAGS) main.cpp -c
isa-top.o: isa-top.cpp isa-top.hpp
	$(CPP) $(CPPFLAGS) isa-top.cpp -c
packet_handler.o: packet_handler.cpp isa-top.hpp packet_handler.hpp
	$(CPP) $(CPPFLAGS) packet_handler.cpp -c

clean:
	rm -rf isa-top *.o