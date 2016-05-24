#
# makefile
#

prog := prog
prog_objs := prog.o

MPICC := mpic++
CFLAGS := -Wall -g -O2 -lm $(NBYTE) $(MPI_BC)


.PHONY: all clean

all: $(prog)

$(prog): heat.cpp
	$(MPICC) $(CFLAGS) heat.cpp -o $@ 


clean:
	@rm -rf *.o $(prog)