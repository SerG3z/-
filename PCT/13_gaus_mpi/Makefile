#
# makefile
#

prog := prog
prog_objs := prog.o

MPICC := mpicc
CFLAGS := -Wall -O2 -std=c11 -lm $(NBYTE) $(MPI_BC)


.PHONY: all clean

all: $(prog)

$(prog):
	$(MPICC) $(CFLAGS) mpi_gauss2.c -o $@ 


clean:
	@rm -rf *.o $(prog)

