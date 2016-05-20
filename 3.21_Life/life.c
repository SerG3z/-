#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpi.h"
#define ALIVE 'X'
#define DEAD '.'

int toindex(int row, int col, int N) 
{
    if (row < 0) {
        row = row + N;
    } else if (row >= N) {
        row = row - N;
    }
    if (col < 0) {
        col = col + N;
    } else if (col >= N) {
        col = col - N;
    }
    return row * N + col;
}

void printgrid(char* grid, char* buf, FILE* f, int N) 
{
    for (int i = 0; i < N; ++i) {
        strncpy(buf, grid + i * N, N);
        buf[N] = 0;
        fprintf(f, "%s\n", buf);
    }
}

int main(int argc, char* argv[]) 
{
    if (argc != 5) {
        fprintf(stderr, "Usage: %s N input_file iterations output_file\n", argv[0]);
        return 1;
    }
    int rank, size;
    MPI_Status status;
    int N = atoi(argv[1]); // grid size
    int iterations = atoi(argv[3]);
     char* grid = NULL; 
    // char* front = NULL;
    char* localbuf = (char*) malloc(N * N * sizeof(char));
    // MPI_Status status, statusfront;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        grid = (char*) malloc(N * N * sizeof(char));
        // front = (char*) malloc(N * N * sizeof(char)); 
        // memset(front, 1, N * N);
        FILE* input = fopen(argv[2], "r");
        
        for (int i = 0; i < N; ++i) {
            fscanf(input, "%s", grid + i * N);
        }
        fclose(input);    
    }
    MPI_Barrier(MPI_COMM_WORLD);
 
    char* localgrid = (char*) malloc(N * N * sizeof(char));
    // char* localfront = (char*) malloc(N * N * sizeof(char));
    char* buf = (char*) malloc (N * N * sizeof(char));
    MPI_Scatter(grid, N * (N / size), MPI_CHAR, localgrid, N * (N / size), MPI_CHAR, 0, MPI_COMM_WORLD);
    // MPI_Scatter(front, N * (N / size), MPI_CHAR, localfront, N * (N / size), MPI_CHAR, 0, MPI_COMM_WORLD);

    for (int iter = 0; iter < iterations; ++iter) {

      if (rank > 0){
            MPI_Send(localgrid, N, MPI_CHAR, rank-1, 1, MPI_COMM_WORLD);
            // MPI_Send(localfront, N, MPI_CHARACTER, rank-1, 1, MPI_COMM_WORLD);
        }

        if (rank < size -1){
            MPI_Recv(localgrid, N, MPI_CHAR, rank + 1, 1, MPI_COMM_WORLD,  &status);
             // MPI_Recv(localfront, N, MPI_CHARACTER, rank+1, 1, MPI_COMM_WORLD,  &statusfront);
        }

        if (rank < size -1){
            MPI_Send(localgrid, N, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);
            // MPI_Send(localfront, N, MPI_CHARACTER, rank + 1, 0, MPI_COMM_WORLD);
        }

        if (rank > size -1){
            MPI_Recv(localgrid, N, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, &status);
            // MPI_Recv(localfront, N, MPI_CHARACTER, rank - 1, 0, MPI_COMM_WORLD, &statusfront);
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int alive_count = 0;
                for (int di = -1; di <= 1; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        if ((di != 0 || dj != 0) && localgrid[toindex(i + di, j + dj, N)] == ALIVE) {
                            ++alive_count;
                        }
                    }
                }
                int current = i * N + j;
                if (alive_count == 3 || (alive_count == 2 && localgrid[current] == ALIVE)) {
                    buf[current] = ALIVE;
                } else {
                    buf[current] = DEAD;
                }
            }
        }     
        char* tmp = localgrid; localgrid = buf; buf = tmp;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(localgrid, N * (N / size), MPI_CHAR, grid, N * (N / size), MPI_CHAR, 0, MPI_COMM_WORLD);
    if(rank == 0){
        FILE* output = fopen(argv[4], "w");
        printgrid(grid, localbuf, output, N);
        fclose(output);

        free(grid);
        free(buf);
        // free(front);
        free(localgrid);
        free(localbuf);
        // free(localfront);
    }
    MPI_Finalize();
    return 0;
}
