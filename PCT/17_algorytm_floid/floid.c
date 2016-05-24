/*
	MPI-программа, реализующая алгоритм Флойда поиска кратчайших путей в графе. 
	Разработать параллельную MPI-программу, реализующую поиск кратчайших путей в ориентированном графе 
	с помощью алгоритма Флойда. Входные данные программы – случайный ориентированный граф со случайными весами рёбер. 
	Граф задан матрицей смежности. Число вершин в графе n = 1000 
	Результат выполнения программы – матрица кратчайших путей между всеми вершинами. 
	Необходимо оценить ускорение параллельной программы.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <mpi.h>
#include <sys/time.h>
#include "hpctimer.h"

#define INF 99 // Отсутствие связи между вершинами

const double ARC_EXISTENCE_PROBABILITY = 0.7;

const int MAX_WEIGHT = 50;

void Read_matrix(int local_mat[], int n, int my_rank, int p,
                 MPI_Comm comm);
void Print_matrix(int local_mat[], int n, int my_rank, int p,
                  MPI_Comm comm);
void Floyd(int local_mat[], int n, int my_rank, int p, MPI_Comm comm);
int Owner(int k, int p, int n);
void Copy_row(int local_mat[], int n, int p, int row_k[], int k);
void Print_row(int local_mat[], int n, int my_rank, int i);
double wtime();

int main(int argc, char* argv[]) {
	int  n = 1000;   //количество вершин
	int* local_mat;
	MPI_Comm comm;
	int p, my_rank;

	MPI_Init(&argc, &argv);
	comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &p);
	MPI_Comm_rank(comm, &my_rank);

	MPI_Bcast(&n, 1, MPI_INT, 0, comm);

	local_mat = (int *) malloc(n * n / p * sizeof(int));

	Read_matrix(local_mat, n, my_rank, p, comm);

	if (my_rank == 0)
		printf("Start ...\n");

	// Print_matrix(local_mat, n, my_rank, p, comm);

	double time;
	
	MPI_Barrier(MPI_COMM_WORLD);

	if (my_rank == 0)
	{
		printf("\n");
		// time = wtime();
		time = hpctimer_getwtime();
	}

	Floyd(local_mat, n, my_rank, p, comm);

	MPI_Barrier(MPI_COMM_WORLD);

	if (my_rank == 0) {
		// printf("time = %.4f \n", wtime() - time);

		printf("time = %.4f \n", hpctimer_getwtime() - time);

		printf("Completed!!!!!\n");
		printf("The solution is:\n");
	}
		
	// Print_matrix(local_mat, n, my_rank, p, comm);

	free(local_mat);
	MPI_Finalize();

	return 0;
} 

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
} 


void Read_matrix(int local_mat[], int n, int my_rank, int p,
                 MPI_Comm comm) {
	int i, j;
	int* temp_mat = NULL;

	if (my_rank == 0) {
		temp_mat = (int *) malloc(n * n * sizeof(int));

		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				// scanf("%d", &temp_mat[i * n + j]);
				if (drand48() < ARC_EXISTENCE_PROBABILITY)
					temp_mat[i * n + j] = lrand48() % MAX_WEIGHT;
				else
					temp_mat[i * n + j] = INF;
			}
		}


		MPI_Scatter(temp_mat, n * n / p, MPI_INT,
		            local_mat, n * n / p, MPI_INT, 0, comm);
		free(temp_mat);
	} else {
		MPI_Scatter(temp_mat, n * n / p, MPI_INT,
		            local_mat, n * n / p, MPI_INT, 0, comm);
	}

}  

void Print_row(int local_mat[], int n, int my_rank, int i) {
	char char_int[100];
	char char_row[1000];
	int j, offset = 0;

	for (j = 0; j < n; j++) {
		if (local_mat[i * n + j] == INF)
			sprintf(char_int, "i ");
		else
			sprintf(char_int, "%d ", local_mat[i * n + j]);
		sprintf(char_row + offset, "%s", char_int);
		offset += strlen(char_int);
	}
	printf("Proc %d > row %d = %s\n", my_rank, i, char_row);
} 


void Print_matrix(int local_mat[], int n, int my_rank, int p,
                  MPI_Comm comm) {
	int i, j;
	int* temp_mat = NULL;

	if (my_rank == 0) {
		temp_mat = (int *) malloc(n * n * sizeof(int));
		MPI_Gather(local_mat, n * n / p, MPI_INT,
		           temp_mat, n * n / p, MPI_INT, 0, comm);
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++)
				if (temp_mat[i * n + j] == INF)
					printf("i ");
				else
					printf("%d ", temp_mat[i * n + j]);
			printf("\n");
		}
		free(temp_mat);
	} else {
		MPI_Gather(local_mat, n * n / p, MPI_INT,
		           temp_mat, n * n / p, MPI_INT, 0, comm);
	}
}  


void Floyd(int local_mat[], int n, int my_rank, int p, MPI_Comm comm) {
	int global_k, local_i, global_j, temp;
	int root;
	int* row_k = (int *) malloc(n * sizeof(int));

	for (global_k = 0; global_k < n; global_k++) {
		root = Owner(global_k, p, n);
		if (my_rank == root)
			Copy_row(local_mat, n, p, row_k, global_k);
		MPI_Bcast(row_k, n, MPI_INT, root, comm);
		for (local_i = 0; local_i < n / p; local_i++)
			for (global_j = 0; global_j < n; global_j++) {
				temp = local_mat[local_i * n + global_k] + row_k[global_j];
				if (temp < local_mat[local_i * n + global_j])
					local_mat[local_i * n + global_j] = temp;
			}
	}
	free(row_k);
}  


int Owner(int k, int p, int n) {
	return k / (n / p);
}

void Copy_row(int local_mat[], int n, int p, int row_k[], int k) {
	int j;
	int local_k = k % (n / p);

	for (j = 0; j < n; j++)
		row_k[j] = local_mat[local_k * n + j];
}