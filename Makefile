CC = gcc
SRC = ./src/*.c
all: 
	$(CC) -c $(SRC) -o ./src/foxevents.o
	ar rcs libfoxevents.a ./src/*.o

	$(CC) -o test test.c -L. -lfoxevents
