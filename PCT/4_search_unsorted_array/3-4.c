#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define BUF_SIZE (long)1e7
#define NTHR 	 3

struct Block {
	long id;
	long *mas;
	long value;
	long pos;
	long start;
	long end;
};

short isFound = 0;

void *thread_func(void *p)
{
	struct Block *b = (struct Block *)p;

	long i;

	for (i = b->start; i < b->end; ++i) {
		if (isFound) break;
		if (b->mas[i] == b->value) {
			printf("Thread #%ld found '%ld' at pos: %ld\n",
				b->id, b->value, b->pos + i + 1);
			isFound = 1;
			break;
		}
	}

	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	srand((unsigned)(time(NULL)));

	long i, j, x;
	long *buf = (long *) malloc(sizeof(long) * BUF_SIZE);

	for (i = 0; i < BUF_SIZE; ++i)
		buf[i] = (long)(100 + rand() % 1337);

//	for (i = 0; i < BUF_SIZE; ++i) {
//		if (!(i % 10) && i)
//			printf("\n");
//
//		printf("[%5ld]", buf[i]);
//	}
//	printf("\n");

	printf("Input x: ");
	scanf("%ld", &x);

	struct Block *b = (struct Block *) malloc(sizeof(struct Block) * NTHR);
	pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * NTHR);

	long N = (BUF_SIZE / NTHR) + 1;

	for (i = 0; i < NTHR; ++i) {
		b[i].id = i;
		b[i].value = x;
		b[i].pos = i * N;
		b[i].start = 0;

		long *a;

		if (i == NTHR - 1) {
			if ((BUF_SIZE % N)) {
				a = (long *) malloc(sizeof(long) * (BUF_SIZE % N));

				for (j = 0; j < (BUF_SIZE % N); ++j)
					a[j] = buf[i * N + j];

				b[i].mas = a;
				b[i].end = BUF_SIZE % N;
			} else {
				a = (long *) malloc(sizeof(long) * N);

				for (j = 0; j < N; ++j)
					a[j] = buf[i * N + j];

				b[i].mas = a;
				b[i].end = N;
			}
		} else {
			a = (long *) malloc(sizeof(long) * N);

			for (j = 0; j < N; ++j)
				a[j] = buf[i * N + j];

			b[i].mas = a;
			b[i].end = N;
		}

//		for (j = 0; j < b[i].end; ++j)
//			printf("[%5ld] ", a[j]);
//		printf("\n");

	}

	clock_t timer = clock();

	for (i = 0; i < NTHR; ++i) {
		int err = pthread_create(&threads[i], NULL, thread_func, (void *)&b[i]);
		if (err) {
			printf("Cannot create this thread: %ld, error code: %d\n", i, err);
			return -1;
		}
		else
			printf("Thread #%ld succesfully created.\n", i);
	}

	for (i = 0; i < NTHR; ++i) {
		pthread_join(threads[i], NULL);
		printf("Thread #%ld end its routine.\n", i);
	}

	if (!isFound) printf("Value '%ld' goes for a shit!\n", x);

	printf("Elapsed time: %f\n", ((float)(clock() - timer)) / CLOCKS_PER_SEC);

	return 0;
}
