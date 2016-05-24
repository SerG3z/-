#
# makefile
#

prog := prog
prog_objs := prog.o

MPICC := mpic++
CFLAGS := -Wall -O2 -lm $(NBYTE) $(MPI_BC)


.PHONY: all clean

all: $(prog)

$(prog):
	$(MPICC) $(CFLAGS) floid.c hpctimer.c -o $@ 


clean:
	@rm -rf *.o $(prog)