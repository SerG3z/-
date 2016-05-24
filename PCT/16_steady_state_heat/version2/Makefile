#
# makefile
#

prog := prog
prog_objs := prog.o

MPICC := mpicc
CFLAGS := -Wall -std=c11 -O2 -lm $(NBYTE) $(MPI_BC)


.PHONY: all clean

all: $(prog)

$(prog): 3_16_mpi_stationary_heat_distribution.c
	$(MPICC) $(CFLAGS) 3_16_mpi_stationary_heat_distribution.c -o $@ 


clean:
	@rm -rf *.o $(prog)