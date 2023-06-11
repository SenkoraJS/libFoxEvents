CC = gcc
SRC = ./src/*.c
all: 
	$(CC) -g -c $(SRC) -o ./src/foxevents.o
	ar rcs libfoxevents.a ./src/*.o

	$(CC) -g -o test test.c -L. -lfoxevents
