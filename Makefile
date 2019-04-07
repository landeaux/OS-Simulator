CPPFLAGS=-std=c++11 -Wall -pthread -lpthread

all: clean sim

MetadataInstruction.o:  MetadataInstruction.h MetadataInstruction.cpp
	g++ -c $(CPPFLAGS) MetadataInstruction.cpp

PCB.o:  PCB.h PCB.cpp
	g++ -c $(CPPFLAGS) PCB.cpp

Timer.o:  Timer.h Timer.cpp
	g++ -c $(CPPFLAGS) Timer.cpp

sim:    MetadataInstruction.o PCB.o Timer.o main.cpp
	g++ -o sim2 $(CPPFLAGS) MetadataInstruction.o PCB.o Timer.o main.cpp

clean:
	rm -f sim2 *.o
