SRC=main.c
CC=gcc
#-Wall
CFLAGS=-O3 -ansi -pedantic
all: clean main
commons.o: ./commons/commons.c
	$(CC) -o commons.o -c ./commons/commons.c $(CFLAGS)
dftmanual.o: ./dftManual/dftmanual.c ./commons/commons.h
	$(CC) -o dftmanual.o -c ./dftManual/dftmanual.c $(CFLAGS)
fftrecursive.o: ./fftRecursive/fftRecursive.c ./commons/commons.h
	$(CC) -o fftrecursive.o -c ./fftRecursive/fftRecursive.c $(CFLAGS)
fftw3impl.o: ./fftw3/fftw3Impl.c ./commons/commons.h
	$(CC) -o fftw3impl.o -c ./fftw3/fftw3Impl.c $(CFLAGS)
main.o:	$(SRC) ./commons/commons.h ./dftManual/dftmanual.h ./fftRecursive/fftRecursive.h ./fftw3/fftw3impl.h
	$(CC) -o main.o -c $(SRC) $(CFLAGS)
main: commons.o dftmanual.o fftrecursive.o fftw3impl.o main.o
	$(CC) -lfftw3 -lm -o main commons.o dftmanual.o fftrecursive.o fftw3impl.o main.o	
.PHONY: clean
clean:
	rm -rf *.o
