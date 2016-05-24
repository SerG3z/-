/*
 * 3.15 MPI-программа умножения матриц. Требуется разработать
 * MPI-программу умножения матриц. Матрица a, b генерируются
 * случайным образом на нулевом процессе. В ходе выполнения
 * умножения загрузка равномерно распределяется между процессами.
 * Результат работы программы – матрица c (результат умножения матриц
 * a и b). При взаимодействии процессов в MPI-программе необходимо
 * использовать виртуальную декартовую топологию вида «двумерная решётка».
 * Необходимо оценить ускорение параллельной программы.
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 3

int a[SIZE][SIZE], b[SIZE][SIZE], c[SIZE][SIZE];

void fill(int m[SIZE][SIZE])
{
	int i, j;
	for (i = 0; i < SIZE; ++i) {
		for (j = 0; j < SIZE; ++j)
			m[i][j] = 1 + rand() % 7;
	}
}

void dump(int m[SIZE][SIZE])
{
	int i, j;
	for (i = 0; i < SIZE; ++i) {
		printf("\n\t| ");
		for (j = 0; j < SIZE; ++j)
			printf("%3d ", m[i][j]);
		printf("|");
	}
}

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	int rank, size, i, j, k, src, dest;
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
		fill(a);
		fill(b);
		timer = clock();
	}

	MPI_Bcast(b, SIZE*SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(a, SIZE*SIZE / size, MPI_INT, a[src], SIZE*SIZE / size,
		MPI_INT, 0, MPI_COMM_WORLD);

	for (i = src; i < dest; ++i) {
		for (j = 0; j < SIZE; ++j) {
//			c[i][j] = 0;
			for (k = 0; k < SIZE; ++k)
				c[i][j] += a[i][k] * b[k][j];
		}
	}

	MPI_Gather(c[src], SIZE*SIZE / size, MPI_INT, c, SIZE*SIZE / size,
		MPI_INT, 0, MPI_COMM_WORLD);

	if (!rank) {
		dump(a);
		printf("\n\n\t		* \n");
		dump(b);
		printf("\n\n\t		= \n");
		dump(c);
		printf("\n\nElapsed time: %f\n", ((float)clock() - timer) / CLOCKS_PER_SEC);
	}

	MPI_Finalize();

	return 0;
}
