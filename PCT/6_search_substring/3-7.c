/*Многопоточная программа, реализующая поиск подстроки в строке.
Требуется разработать многопоточную программу поиска строки d в строке s. Строка s
формируется случайным образом, строка d вводится с клавиатуры. Строка s равномерно
распределяется между потоками. При нахождении строки d одним из потоков, выполнение
поиска прекращается всеми потоками. Выходные данные программы – индекс в массиве s,
с которого начинается строка d, или –1, если подстрока не обнаружена.
Необходимо оценить ускорение параллельной программы.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define MAX_SLEN 1e7
#define MAX_DLEN 256
#define NTHR 	 3

struct Block {
	long id;

	char *strS;
	char *strD;
	long start;
	long end;
};

short isFound = 0;

void *thread_func(void *p)
{
	long i, j, k;

	struct Block *b = (struct Block *)p;

	long lenS = b->end - b->start;
	long lenD = strlen(b->strD);

	int *d = (int *) malloc(sizeof(int) * lenD);

	d[0] = 0;

	for (i = 1, j = 0; i < lenD; ++i) {
		if (isFound) break;
		while (j > 0 && b->strD[j] != b->strD[i]) {
			if (isFound) break;
			j = d[j - 1];
		}

		if (b->strD[j] == b->strD[i])
			++j;

		d[i] = j;
	}

	for (i = b->start, j = 0; i < b->end; ++i) {
		if (isFound) break;
		while (j > 0 && b->strD[j] != b->strS[i]) {
			if (isFound) break;
			j = d[j - 1];
		}

		if (b->strD[j] == b->strS[i])
			++j;

		if (j == lenD) {
			printf("Thread #%ld found '%s' at pos: %ld\n", b->id, b->strD, i - j + 1);
			isFound = 1;
			break;
		}
	}

	free(d);
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	srand((unsigned)(time(NULL)));

	long i, j, k;

	char *s = (char *) malloc(sizeof(char) * MAX_SLEN);
	char *d = (char *) malloc(sizeof(char) * MAX_DLEN);

	for (i = 0; i < MAX_SLEN; ++i)
		s[i] = (char)(65 + rand() % 50);

//	printf("s: %s\n", s);

	printf("Input d: ");
	fgets(d, MAX_DLEN, stdin);
	d[strlen(d) - 1] = '\0';

	struct Block *b = (struct Block *) malloc(sizeof(struct Block) * NTHR);
	pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * NTHR);

	long N = 1 + (strlen(s) / NTHR);

	for (i = 0; i < NTHR; ++i) {
		b[i].id = i;
		b[i].strS = s;
		b[i].strD = d;
		b[i].start = i * N;

		if (i == NTHR - 1)
			b[i].end = strlen(s) - 1;
		else
			b[i].end = i * N + strlen(d) + N - 1;
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

	if (!isFound) printf("String '%s' goes for a shit!\n", d);

	printf("Elapsed time: %f\n", ((float)(clock() - timer)) / CLOCKS_PER_SEC);

	return 0;
}
