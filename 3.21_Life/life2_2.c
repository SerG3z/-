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

    int N = atoi(argv[1]); // grid size
    int iterations = atoi(argv[3]);
    int rank, size;
    MPI_Status status;
    
    char* grid = (char*) malloc(N * N * sizeof(char));
    char* buf = (char*) malloc(N * N * sizeof(char));
    char* front = (char*) malloc(N * N * sizeof(char));
    memset(front, 1, N * N);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sendcnts[size], displs[size];

    for (int i = 0, offset = 0; i < size; i++) {
        sendcnts[i] = N * (N / size);
        displs[i] = offset;
        offset += N * (N / size);
    }
    
    char* local_grid = (char*) malloc((N * (N / size) + 2 * N) * sizeof(char));
    char* local_buf = (char*) malloc((N * (N / size) + 2 * N) * sizeof(char));
    char* local_front = (char*) malloc((N * (N / size) + 2 * N) * sizeof(char));

    if (rank == 0) {
        FILE *input;
        input = fopen(argv[2], "r");
            if (!input)
                MPI_Abort(MPI_COMM_WORLD, 1);
        for (int i = 0; i < N; ++i) {
            fscanf(input, "%s", grid + i * N);
        }
    }
    int centre = sendcnts[rank];

    MPI_Barrier(MPI_COMM_WORLD);    
    MPI_Scatterv(grid, sendcnts, displs, MPI_CHAR, local_grid + N * sizeof(char), N * (N / size), MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Scatterv(front, sendcnts, displs, MPI_CHAR, local_front + N * sizeof(char), N * (N / size), MPI_CHAR, 0, MPI_COMM_WORLD);

    for (int iter = 0; iter < iterations; ++iter) {
        MPI_Send(local_grid + N, N, MPI_CHAR, (size + rank - 1) % size, 0, MPI_COMM_WORLD);
        MPI_Recv(local_grid + N + centre, N, MPI_CHAR, (rank + 1) % size, 0, MPI_COMM_WORLD, &status);

        MPI_Send(local_grid + centre, N, MPI_CHAR, (rank + 1) % size, 1, MPI_COMM_WORLD);
        MPI_Recv(local_grid, N, MPI_CHAR, (size + rank - 1) % size, 1, MPI_COMM_WORLD, &status);

        int next_front = (iter + 1) % 2 + 1;
        for (int i = 1; i < centre / N + 1; ++i) {
            for (int j = 0; j < N; ++j) {
                int current = i * N + j;
                if (local_front[current] != 0) {
                    int alive_count = 0;
                    for (int di = -1; di <= 1; ++di) {
                        for (int dj = -1; dj <= 1; ++dj) {
                            if ((di != 0 || dj != 0) && local_grid[toindex(i + di, j + dj, N)] == ALIVE){
                                ++alive_count;
                            }
                        }
                    }
                    if (alive_count == 3 || (alive_count == 2 && local_grid[current] == ALIVE)) {
                        local_buf[current] = ALIVE;
                    } else {
                        local_buf[current] = DEAD;
                    }
                    if (local_buf[current] != local_grid[current]) {
                        for (int di = -1; di <= 1; ++di) {
                            for (int dj = -1; dj <= 1; ++dj) {
                                local_front[toindex(i + di, j + dj, N)] = next_front;
                            }
                        }
                    } else {
                        if (local_front[current] != next_front) {
                            local_front[current] = 0;
                        }
                    }
                }
            }
        }

        for (int i = 1; i < centre / N + 1; i += centre / N - 1) {
            for (int j = 0; j < N; ++j) {
                local_front[i * N + j] = 1;
            }
        }
        char* tmp = local_grid; local_grid = local_buf; local_buf = tmp;
    }

    MPI_Gatherv(local_grid + N, centre, MPI_CHAR, grid, sendcnts, displs, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        FILE* output = fopen(argv[4], "w");
        printgrid(grid, buf, output, N);
        fclose(output);
        free(grid);
        free(buf);
        free(front);
        free(local_grid);
        free(local_buf);
        free(local_front);
    }

    MPI_Finalize();
    return 0;
}
