/* 3.13 Многопоточная программа редукции элементов 
 * массива Реализовать параллельную программу выполнения 
 * коллективной операции редукции для заданной 
 * ассоциативной операции. На вход программы подаётся 
 * операция  и случайно сформированный массив a. 
 * Выходные данные программы –результат выполнения редукции (число). 
 * Необходимо оценить ускорение параллельной программы. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

void reduce_sequential(int *v, int n, long *sum)
{
    long s = 0;
    for (int i = 0; i < n; i++)
        s += (long) v[i];
    *sum = s;
}

void reduce_parallel(int *v, int n, long *sum)
{
    long s = 0;
    int nthreads = omp_get_max_threads();

#pragma omp parallel for reduction(+:s)
    for (int i = 0; i < n; i++)
        s += v[i];
    *sum = s;
}

int main(int argc, const char *argv[])
{
    long sum;
    int n;

//    printf("N = ");
//    scanf("%d", &n);
    n = 300000000;

    int *v = malloc(sizeof(int) * n);
    if (v == NULL) return 1;

    for (int i = 0; i < n; i++) {
        v[i] = i;
    }

    double t = wtime();
    reduce_sequential(v, n, &sum);
    t = wtime() - t;
    printf("sum sequential: %ld, Execution time: %.6f\n", sum, t);

    t = wtime();
    reduce_parallel(v, n, &sum);
    t = wtime() - t;
    printf("sum parallel: %ld, Execution time: %.6f\n", sum, t);

    return 0;
}
