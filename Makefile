
all: clean test5

test5: main.cpp
	g++ -std=c++11 -Wall -o sim1 main.cpp

clean:
	rm -f sim1