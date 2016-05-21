CC=gcc
CPP=g++
CFLAGS=-Wall -g -fopenmp -o 

all:
	make prog_01 &&\
	make prog_02 &&\
        make prog_03


prog_01:
	$(CC) $(CFLAGS) prog_01.out prog_01.c -lm

prog_02:
	$(CPP) $(CFLAGS) prog_02.out prog_02.cpp

prog_03:
	$(CPP) $(CFLAGS) prog_03.out prog_03.cpp


clear:
	rm *.out *.txt
