#include <unistd.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <png.h>

#define SIZE 400
#define REPEAT_COUNT 1

int set[SIZE][SIZE];
pthread_mutex_t setWrite = PTHREAD_MUTEX_INITIALIZER;

struct threadArguments {
    int startline;
    int endline;
    double x1, y1, x2, y2;
};

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

void printSet(FILE *file) {
    for (int j = 0; j < SIZE; ++j){
        for (int i = 0; i < SIZE; ++i){
            fprintf(file, "%.4d  ", set[i][j]);
        }
       fprintf(file, "\n");
    }
}

void *mandelbrot(void *args)
{
    struct threadArguments *a = (struct threadArguments *)args;
    double width = a->x2 - a->x1;
    double height = a->y1 - a->y2;

    //For each point kon the complex plane do:
    //for (int i = 0; i < SIZE; i++) {
    for (int i = a->startline; i < a->endline; i++) { // because of paralleling
        for (int j = 0; j < SIZE; j++) {

            double x = (double)i / (double) SIZE * width + a->x1;
            double y = (double)j / (double) SIZE * height + a->y2;

            double z_re = x;
            double z_im = y;

            int k;
            //    repeat 4000 times:
            for (k = 0; k < 4000; ++k) {
                // x=x^2+k in complex numbers
                double z_re_new =  z_re * z_re - z_im * z_im + x;
                double z_im_new =  2 * z_re * z_im + y;
                z_re = z_re_new;
                z_im = z_im_new;

                if (z_re * z_re + z_im * z_im > 4.0) {
                    break;
                }
            } // end repeat

            set[i][j] = k;
        }
    }
    return NULL;

}

double calculate(int threadCount) {
    double time = wtime();

    pthread_t threads[threadCount];
    struct threadArguments *args;
    int linesByThread = SIZE / (threadCount);
    int startline = 0;
    int endline = 0;
    // раздаем потокам их часть работы
    int i = 0;
    while(endline < SIZE - 1) {

        startline = endline + 1;
        endline += linesByThread;
        if (endline > SIZE - 1) {
            endline = SIZE - 1;
        }

        args = malloc(sizeof(struct threadArguments));
        args->x1 = -2.0;
        args->y1 = 1.0;

        args->x2 = 1.0;
        args->y2 = -1.0;

        args->startline = startline;
        args->endline = endline;
        pthread_create(&threads[i], NULL, mandelbrot, args);

        i++;
    }

    for (i = 0; i < threadCount; ++i){
        pthread_join(threads[i], NULL);
    }
    time = wtime() - time;

    char filename[20];
    FILE *current_file;
    sprintf(filename, "%.2d_output", threadCount);
    current_file = fopen(filename, "w");
    printSet(current_file);
    fclose(current_file);

    return time;

}

int main(int argc, char **argv)
{
    FILE *data;
    data = fopen("results.txt", "w");

    double averageTime = 0;
    double oneThreadTime = averageTime;

    if (argc != 1) {
       printf("Usage:\n %s <x1> <y1> <x2> <y2> floats\n", argv[0]);
       return 0;
    }

    printf("Repeat counts is %d\n", REPEAT_COUNT);
    printf("N | Average Time | Accelerate\n");
    for (int thread = 1; thread < 10; ++thread){
        averageTime = 0;
        for (int try = 0; try < REPEAT_COUNT; ++try){
            averageTime += calculate(thread);
        }
        averageTime /= REPEAT_COUNT;
        if (thread == 1) {
            oneThreadTime = averageTime;
        }

        printf("%d | %.4f        | %.4f \n", thread, averageTime, oneThreadTime / averageTime);
        fprintf(data, "%d %f\n", thread, oneThreadTime / averageTime);
    }

    fclose(data);
    return 0;
}
