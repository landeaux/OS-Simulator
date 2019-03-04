
all: clean sim

sim: main.cpp
	g++ -std=c++11 -Wall -o sim2 main.cpp

clean:
	rm -f sim2