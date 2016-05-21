#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <omp.h>
#include <sys/time.h>

#define STEP     0.001
#define N        40

double a = 0, b = 0;
int    NUM_THREADS = 1;

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int main (int argc, char *argv[])
{
    if (argc >= 3) {
        a = atof (argv[1]);
        b = atof (argv[2]);
    } else {
        printf("Give me please arguments(a, b, NUM_THREADS)\n");
        return 0;
    }

    if (argc == 4) {
        NUM_THREADS = atoi (argv[3]);
    }

    double arg;
    double im;
    double re;
    int    iter;

    FILE  *fp;

    fp = fopen ("res_01.txt", "w");

    double t = wtime();

    #pragma omp parallel num_threads (NUM_THREADS)
    {
        #pragma omp for collapse (2) schedule (guided, 2)
        for ( int i = (int) ((0.00 - a / 2.00) / STEP);
                i < (int) ((a / 2.00) / STEP)           ;
                ++i                                       ) {
            for ( int j = (int) ((0.00 - b / 2.00) / STEP);
                    j < (int) ((b / 2.00) / STEP)           ;
                    ++j                                       ) {

                iter = 0;
                re = (double) i * STEP;
                im = (double) j * STEP;
                arg = pow ((double) i * STEP, 2.00) +
                      pow ((double) j * STEP, 2.00);

                while (arg <= 4.00 && iter < N) {
                    im = (2.00 * re * im) + (double) j * STEP;
                    re = pow (re, 2.00) - pow (im, 2.00) + (double) i * STEP;
                    arg = pow (re, 2.00) + pow (im, 2.00);
                    ++iter;
                }

                if (arg <= 4.00 && j < 0) {
                    fprintf ( fp, "(%.3lf + (%.3lf) * i)\n",
                              (double) i * STEP, (double) j * STEP);
                } else if (arg <= 4.00 && j > 0) {
                    fprintf ( fp, "(%.3lf + %.3lf * i)\n",
                              (double) i * STEP, (double) j * STEP);
                }
            }
        }
    }

    t = wtime() - t;

    printf("time = %.6f\n", t);

    fclose (fp);

    return 0;
}
