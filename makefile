SRC=main.c
CC=gcc
CFLAGS=-O3 -Wall -ansi -pedantic
all: clean main
commons.o: ./commons/commons.c
	$(CC) -o commons.o -c ./commons/commons.c $(CFLAGS)
dftmanual.o: ./dftManual/dftmanual.c ./commons/commons.h
	$(CC) -o dftmanual.o -c ./dftManual/dftmanual.c $(CFLAGS)
main.o:	$(SRC) ./commons/commons.h ./dftManual/dftmanual.h
	$(CC) -o main.o -c $(SRC) $(CFLAGS)
main: commons.o dftmanual.o main.o
	$(CC) -o main commons.o dftmanual.o main.o	
.PHONY: clean
clean:
	rm -rf *.o
