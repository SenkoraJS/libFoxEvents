CXX = g++
SRC = ./src/*.cpp
all: 
	$(CXX) -g -c $(SRC) -o ./src/foxevents.o
	ar rcs libfoxevents.a ./src/*.o

	$(CXX) -g -o test test.cpp -L. -lfoxevents
