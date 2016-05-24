#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

//#define BUF_SIZE (long)1e7
#define BUF_SIZE 99
#define NTHR 	 3

struct Block {
	long id;	// thread id
	long *mas;	// subarray
	long idx;	// thread position in subarray
	long value;	// X
	long pos;	// thread position in array
	long start;	// subarray start
	long end;	// subarray end

	int flag;	// -1: X on the left
				//  1: X on the right
				//  0: THAT`s X, baby
};

short isFound = 0;

// for sorting shit
int cmp(const void *a, const void *b)
{
	return *(long *)a - *(long *)b;
}

// just compare thread position in subarray with X
// if it`s > X: flag = -1
// if it`s < X: flag = 1
// if it`s = X: flag = 0
void *thread_func(void *p)
{
	struct Block *b = (struct Block *)p;

	long i;

	printf("Thread #%ld stay on %ld(%ld).\n", b->id, b->pos, b->mas[b->idx]);
	printf("Thread idx: %ld\n", b->idx);

	if (b->mas[b->idx] < b->value)
		b->flag = 1;
	else if (b->mas[b->idx] > b->value)
		b->flag = -1;
	else {
		b->flag = 0;
		printf("Thread #%ld found '%ld' at pos: %ld\n",
			b->id, b->value, b->pos + 1);
		isFound = 1;
	}
	printf("\twith flag: %d\n", b->flag);

//	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	srand((unsigned)(time(NULL)));

	long i, j, x;
	long *buf = (long *) malloc(sizeof(long) * BUF_SIZE);

	for (i = 0; i < BUF_SIZE; ++i)
		buf[i] = (long)(100 + rand() % 1337);

	qsort(buf, BUF_SIZE, sizeof(long), cmp);

	// print array
	for (i = 0; i < BUF_SIZE; ++i) {
		if (!(i % 10) && i)
			printf("\n");

		printf("[%5ld]", buf[i]);
	}
	printf("\n");

	printf("Input x: ");
	scanf("%ld", &x);

	struct Block *b = (struct Block *) malloc(sizeof(struct Block) * NTHR);
	pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * NTHR);

	long N = BUF_SIZE / (NTHR);		// number of elements in subarray

	long left_border = 0;			// this shit need to us when we choose
									// what subarray lookup next
									// we start from that position and do partitioning

	clock_t timer = clock();

	// do til the element not found
	while (!isFound) {
		printf("\n\nN: %ld.\n\n", N);

		for (i = 0; i < NTHR; ++i) {
			b[i].id = i;
			b[i].value = x;
			b[i].idx = N - 1;		// thread position always point at the last element of subarray
			b[i].start = 0;

			long *a;

			if (i == NTHR - 1) {
				// if its last - we need to allocate more than N elements (N + BUF_SIZE % N)
				if ((BUF_SIZE % N)) {
					a = (long *) malloc(sizeof(long) * (BUF_SIZE % N + N));

					for (j = 0; j < (BUF_SIZE % N) + N; ++j)
						a[j] = buf[i * N + j + left_border];

					b[i].pos = i * N + N - 1 + (BUF_SIZE % N);
					printf("b[%ld].pos: %ld\n", i, b[i].pos);
					b[i].mas = a;
					b[i].end = BUF_SIZE % N + N;
				} else {
					// that`s just for case when BUF_SIZE % N == 0 (allocate like it`s normal shit)
					a = (long *) malloc(sizeof(long) * N);

					for (j = 0; j < N; ++j)
						a[j] = buf[i * N + j + left_border];

					b[i].pos = i * N + N - 1;
					printf("b[%ld].pos: %ld\n", i, b[i].pos);
					b[i].mas = a;
					b[i].end = N;
				}
			} else {
				// normal shit
				a = (long *) malloc(sizeof(long) * N);

				for (j = 0; j < N; ++j)
					a[j] = buf[i * N + j + left_border];

				b[i].pos = i * N + N - 1;
				printf("b[%ld].pos: %ld\n", i, b[i].pos);
				b[i].mas = a;
				b[i].end = N;
			}

			for (j = 0; j < b[i].end; ++j)
				printf("[%5ld] ", a[j]);
			printf("\n");
		}

		// start multithreading
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

		// when we do partition of array on subarrays we need stop do that onetime
		// if we cannot find element X and number of elements in each subarray = 1
		// we can`t do more partitioning shit then so just break
		if (N == 1) break;

		for (i = 0; i < NTHR; ++i) {
			// we need to find subarray in which our element might be
			// so for do that we need to find THE MOTHERFUCKING RIGHTMOST BORDER
			// so we just start our partitioning from it
			if (b[i].flag == -1 && b[i].pos > left_border) {
				left_border = b[i].pos + 1;
			}
			else if (!b[i].flag) {
				printf("Elapsed time: %f\n", ((float)(clock() - timer)) / CLOCKS_PER_SEC);
				return 0;
			}
			else continue;
		}

		// just because we starts from huge array
		// and then we need to reduce our range of subarrays
		N /= (NTHR);
	}

	if (!isFound) printf("Value '%ld' goes for a shit!\n", x);

	printf("Elapsed time: %f\n", ((float)(clock() - timer)) / CLOCKS_PER_SEC);


	return 0;
}
