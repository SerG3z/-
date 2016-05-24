prog := prog
prog_objs := dgemm.o hpctimer.o

CC := gcc
CFLAGS := -O0 -Wall -g -fopenmp
LDFLAGS := -fopenmp

.PHONY: all clean

all: $(prog)

$(prog): $(prog_objs)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

dgemm.o: dgemm.c
hpctimer.o: hpctimer.c hpctimer.h

clean:
	@rm -rf *.o $(prog)
