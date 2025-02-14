CPPFLAGS=-std=c++11 -Wall -pthread

all: clean sim

Config.o:  Config.h Config.cpp
	g++ -c $(CPPFLAGS) Config.cpp

MetadataInstruction.o:  MetadataInstruction.h MetadataInstruction.cpp
	g++ -c $(CPPFLAGS) MetadataInstruction.cpp

Metadata.o:  MetadataInstruction.h Metadata.h Metadata.cpp
	g++ -c $(CPPFLAGS) Metadata.cpp

PCB.o:  PCB.h PCB.cpp
	g++ -c $(CPPFLAGS) PCB.cpp

Process.o:  Process.h Process.cpp
	g++ -c $(CPPFLAGS) Process.cpp

Timer.o:  Timer.h Timer.cpp
	g++ -c $(CPPFLAGS) Timer.cpp

helpers.o:  helpers.h helpers.cpp
	g++ -c $(CPPFLAGS) helpers.cpp

Simulation.o:  Config.h MetadataInstruction.h Metadata.h PCB.h Process.h Timer.h Simulation.h Simulation.cpp
	g++ -c $(CPPFLAGS) Simulation.cpp

sim:    helpers.o Config.o MetadataInstruction.o Metadata.o PCB.o Process.o Timer.o Simulation.o main.cpp
	g++ -o sim4 $(CPPFLAGS) helpers.o Config.o MetadataInstruction.o \
	Metadata.o PCB.o Process.o Timer.o Simulation.o main.cpp

clean:
	rm -f sim4 *.o
