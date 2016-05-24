/* 
 * dgemm.c: DGEMM - Double-precision General Matrix Multiply.
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#include "hpctimer.h"

enum {
    N = 1024,
    NREPS = 5
};

double A[N * N], B[N * N], C[N * N];

void dgemm_def(double *a, double *b, double *c, int n)
{
    int i, j, k;
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
                *(c + i * n + j) += *(a + i * n + k) * *(b + k * n + j);
            }
        }
    }
}

void dgemm_block(double *a, double *b, double *c, int n)
{
    int i, j, k, i0, j0, k0;
    int BS = 2;
    double *c0, *a0, *b0;
    
    for (i = 0; i < n; i += BS) {
        for (j = 0; j < n; j += BS) {
            for (k = 0; k < n; k += BS) {
                for (i0 = 0, c0 = (c + i * n + j), a0 = (a + i * n + k); i0 < BS; ++i0, c0 += n, a0 += n) {
                    for (k0 = 0, b0 = (b + k * n + j); k0 < BS; ++k0, b0 += n) {
                        for (j0 = 0; j0 < BS; ++j0) {
                            c0[j0] += a0[k0] * b0[j0];
                        }
                    }
                }
            }
        }
    }
}

void dgemm_block_omp(double *a, double *b, double *c, int n)
{
    int i, j, k, i0, j0, k0;
    int BS = 2;
    double *c0, *a0, *b0;
    int nthr, chunk;
    
    nthr = omp_get_max_threads();
//    printf("nthr %d\n", nthr);
//    printf("%d / %d %d\n",n, nthr, n / nthr);
    chunk = n / (nthr * 4);

    #pragma omp parallel for num_threads(nthr) private(i, j, k, i0, j0, k0, c0, a0, b0) schedule(dynamic, chunk)
    for (i = 0; i < n; i += BS) {
        for (j = 0; j < n; j += BS) {
            for (k = 0; k < n; k += BS) {
                for (i0 = 0, c0 = (c + i * n + j), a0 = (a + i * n + k); i0 < BS; ++i0, c0 += n, a0 += n) {
                    for (k0 = 0, b0 = (b + k * n + j); k0 < BS; ++k0, b0 += n) {
                        for (j0 = 0; j0 < BS; ++j0) {
                            c0[j0] += a0[k0] * b0[j0];
                        }
                    }
                }
            }
        }
    }
}

void init_matrix(double *a, double *b, double *c, int n)
{
    int i, j, k;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
                *(a + i * n + j) = 1.0;
                *(b + i * n + j) = 2.0;
                *(c + i * n + j) = 0.0;
            }
        }
    }
}

int validate_dgemm(double *a, double *b, double *c, int n)
{
    int i;
    double eps = 0.000001;
    double* tmp = (double*) malloc(sizeof(double) * n * n);
    
    dgemm_def(a, b, tmp, n);
    
    printf("Start validation...\n");
    
    for (i = 0; i < n * n; i++) {
        if ( fabs((tmp[i] * NREPS) - c[i]) > eps ) {
            fprintf(stderr, "%s\n", "Error");
            return 1;
        }
    }
    printf("Success\n");
    free(tmp);
    
    return 0;
}

void print_matrix(double *a, int n)
{
    int i, j;
    
    printf("Matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%12.2f", *(a + i * n + j));
        }
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    int i;
    double t;
            
    init_matrix(A, B, C, N);
        
    t = hpctimer_getwtime();
    for (i = 0; i < NREPS; i++) {
        dgemm_block_omp(A, B, C, N);
    }
    t = hpctimer_getwtime() - t;
    t = t / NREPS;
    
//    validate_dgemm(A, B, C, N);
    /*print_matrix(C, N);*/
    
    printf("Elapsed time: %.6f sec.\n", t);

    return 0;
}

