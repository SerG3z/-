COMPILER=gcc
CFLAGS=-g -Wall -pthread -std=c99
SOURSES=main.c
OBJECTS=$(SOURSES:.c=.o)
EXECUTABLE=mandelbrot

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURSES)
	$(COMPILER) $(CFLAGS) $(SOURSES) -o $@

$(OBJECTS):$(SOURSES)
	$(COMPILER) $(CFLAGS) -c $(SOURSES)

clean:
	rm $(OBJECTS) $(EXECUTABLE) results.txt *_output graph.png

job: all
	qsub task.job

run: all
	./mandelbrot
	./plot.sh
