SRC=main.c
CC=gcc
CFLAGS=-O3 -Wall -ansi -pedantic
all: clean main
commons.o: ./commons/commons.c
	$(CC) -o commons.o -c ./commons/commons.c $(CFLAGS)
dftmanual.o: ./dftManual/dftmanual.c ./commons/commons.h
	$(CC) -o dftmanual.o -c ./dftManual/dftmanual.c $(CFLAGS)
fftrecursive.o: ./fftRecursive/fftRecursive.c ./commons/commons.h
	$(CC) -o fftrecursive.o -c ./fftRecursive/fftRecursive.c $(CFLAGS)
main.o:	$(SRC) ./commons/commons.h ./dftManual/dftmanual.h ./fftRecursive/fftRecursive.h
	$(CC) -o main.o -c $(SRC) $(CFLAGS)
main: commons.o dftmanual.o fftrecursive.o main.o
	$(CC) -o main commons.o dftmanual.o fftrecursive.o main.o	
.PHONY: clean
clean:
	rm -rf *.o
