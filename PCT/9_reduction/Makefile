prog := prog
prog_objs := reduction.o

CC := gcc
CFLAGS := -std=c11 -g -O0 -fopenmp
LDFLAGS := -lm -fopenmp

.PHONY: all clean

all: $(prog)

$(prog): $(prog_objs)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

reduction.o: reduction.c

clean:
	@rm -rf *.o $(prog)
