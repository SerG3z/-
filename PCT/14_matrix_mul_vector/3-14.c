/*
 * 3.14 MPI-программа умножения матрицы на вектор.
 * Требуется разработать MPI-программу умножения матрицы
 * на вектор. Матрица a и вектор b генерируются случайным
 * образом на нулевом процессе. В ходе выполнения умножения
 * загрузка равномерно распределяется между процессами.
 * Результат работы программы – вектор c (результат умножения).
 * При взаимодействии процессов в MPI-программе необходимо использовать
 * виртуальную декартовую топологию вида «двумерная решётка».
 * Необходимо оценить ускорение параллельной программы.
 */
 
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 3

int a[SIZE][SIZE], b[SIZE], c[SIZE];

void fill(int m[SIZE][SIZE], int v[SIZE])
{
	int i, j;
	for (i = 0; i < SIZE; ++i) {
		for (j = 0; j < SIZE; ++j) {
				if (!i)
					v[j] = rand() % 10;
			m[i][j] = rand() % 20;
		}
	}
}

void dumpM(int m[SIZE][SIZE])
{
	int i, j;
	for (i = 0; i < SIZE; ++i) {
		printf("\n\t| ");
		for (j = 0; j < SIZE; ++j)
			printf("%3d ", m[i][j]);
		printf("|");
	}
}

void dumpV(int v[SIZE])
{
	int i;
	printf("\n\t| ");
	for (i = 0; i < SIZE; ++i)
		printf("%3d ", v[i]);
	printf("|");
}

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	int rank, size, i, j, k, src, dest, sz;
	clock_t timer;

	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);

	if (SIZE % size) {
		if (!rank)
			fprintf(stderr, "Not enough processors!\n");
		MPI_Finalize();
		return -1;
	}

	src = rank * SIZE / size;
	dest = (rank + 1) * SIZE / size;

	if (!rank) {
	  fill(a, b);
	  timer = clock();
	}

	MPI_Bcast(b, SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(a, SIZE*SIZE / size, MPI_INT, &a[src], SIZE*SIZE / size,
		MPI_INT, 0, MPI_COMM_WORLD);

	for (i = src; i < dest; ++i) {
		for (j = 0; j < SIZE; ++j)
			c[i] += a[i][j] * b[j];
	}

	MPI_Gather(&c[src], SIZE / size, MPI_INT, c, SIZE / size,
		MPI_INT, 0, MPI_COMM_WORLD);

	if (!rank) {
		dumpM(a);
		printf("\n\n\t		* \n");
		dumpV(b);
		printf("\n\n\t		= \n");
		dumpV(c);
		printf("\n\nElapsed time: %f\n", ((float)clock() - timer) / CLOCS_PER_SEC);
	}

	MPI_Finalize();

	return 0;
}
