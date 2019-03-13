CPPFLAGS=-std=c++11 -Wall -pthread

all: clean sim

sim: main.cpp
	g++ $(CPPFLAGS) -o sim2 main.cpp PCB.cpp MetadataInstruction.cpp Timer.cpp

clean:
	rm -f sim2