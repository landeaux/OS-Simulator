CPPFLAGS=-std=c++11 -Wall -pthread

all: clean sim

Config.o:  Config.h Config.cpp
	g++ -c $(CPPFLAGS) Config.cpp

MetadataInstruction.o:  MetadataInstruction.h MetadataInstruction.cpp
	g++ -c $(CPPFLAGS) MetadataInstruction.cpp

PCB.o:  PCB.h PCB.cpp
	g++ -c $(CPPFLAGS) PCB.cpp

Timer.o:  Timer.h Timer.cpp
	g++ -c $(CPPFLAGS) Timer.cpp

helpers.o:  helpers.h helpers.cpp
	g++ -c $(CPPFLAGS) helpers.cpp

sim:    helpers.o Config.o MetadataInstruction.o PCB.o Timer.o main.cpp
	g++ -o sim3 $(CPPFLAGS) helpers.o Config.o MetadataInstruction.o PCB.o \
	Timer.o main.cpp

clean:
	rm -f sim3 *.o
