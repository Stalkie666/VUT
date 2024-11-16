# Author: Jakub Hamadej, xhamad03@vutbr.cz, FIT VUT, 11/11/2024

CPP=g++
CPPFLAGS=-Wall -pedantic -lncurses -lpcap

default: isa-top
	
isa-top: isa_top.o packet_handler.o main.o record.o
	$(CPP) $(CPPFLAGS) record.o isa_top.o packet_handler.o main.o -o isa-top

main.o: main.cpp isa_top.hpp packet_handler.hpp
	$(CPP) $(CPPFLAGS) main.cpp -c
isa_top.o: isa_top.cpp isa_top.hpp record.hpp
	$(CPP) $(CPPFLAGS) isa_top.cpp -c
record.o: record.cpp record.hpp
	$(CPP) $(CPPFLAGS) record.cpp -c
packet_handler.o: packet_handler.cpp isa_top.hpp packet_handler.hpp
	$(CPP) $(CPPFLAGS) packet_handler.cpp -c


pack:
	zip isa-top.zip Makefile *.cpp *.hpp isa-top.1

clean:
	rm -rf isa-top *.o isa-top.zip