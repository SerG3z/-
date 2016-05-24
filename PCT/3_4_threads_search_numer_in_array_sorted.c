#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#define N_THREADS 64
#define SIZE_ARRAY 1000000

pthread_t thrs[N_THREADS];
pthread_barrier_t barrier;

int array[SIZE_ARRAY];
int flag_cancel[N_THREADS]; /* флаг завершения потока */
int number_x;

struct info_search {
    int pos;
    int size;
} info[N_THREADS];

void init_array(int *array, int size){
    int i, tmp_val, rand_i;
    
    //последовательный массив
    for (i = 0; i < size; i++) {
        array[i] = i;
    }

    /*
    for (i = 0; i < size; i++) {
        printf("a[%i] = %i\n", i, array[i]);
    }
    */
}

void *search_number(void *ptr) {
    int index = (intptr_t) ptr;
    int i, k;
    
    /* Поток может быть завершен другим потокам */
    if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) != 0) {
        perror("setcancelstate");
        exit(0);
    }

    if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0) {
        perror("setcanceltype");
        exit(0);
    }
    
    flag_cancel[index] = 1;
    
    pthread_barrier_wait(&barrier);
    
    k = 0;
    
    for (i = info[index].pos; k < info[index].size; k++, i++) {
        if (array[i] == number_x) {
            //printf("number_x found! index = %d index_treads = %d!\n", i, index);
            pthread_t id = pthread_self();
            for (i = 0; i < N_THREADS; i++) {
                if (thrs[i] != id && flag_cancel[i] == 1) {
                    pthread_cancel(thrs[i]);
                }
            }
            break;
        }
    }
    
    flag_cancel[index] = 0;
    pthread_exit((void*)NULL);
}

int main_func() {
    int i;
    
    pthread_barrier_init(&barrier, NULL, N_THREADS);
    
    for (i = 0; i < N_THREADS; i++) {
        if (pthread_create(&thrs[i], NULL, search_number, (void *)(intptr_t)i) != 0) { 
            perror("Cannot create a thread");
            return -1;
        } 
    }
    
    for (i = 0; i < N_THREADS; i++) {
        if (pthread_join(thrs[i], NULL) != 0) {
            perror("Cannot join a thread");
            return -1;
        }
    }
    
    
    return 0;
}

int main() {
    int i, n_tests = 100, tmp_pos; 
    float val_time, avg_time = 0;
    struct timeval tv, tv1;
    
    srand(time(NULL));
    init_array(array, SIZE_ARRAY);
    
    /* Распределение массива между потоками */
    tmp_pos = 0;
    for (i = 0; i < N_THREADS; i++) {
        if ( (SIZE_ARRAY % N_THREADS == 0) || 
             ((SIZE_ARRAY % N_THREADS) / (i + 1) == 0) ) {
            info[i].size = SIZE_ARRAY / N_THREADS;
        } else {
            info[i].size = SIZE_ARRAY / N_THREADS + 1;
        }
        info[i].pos = tmp_pos;
        tmp_pos += info[i].size;
    }
    
    //printf("Input number: ");
    //scanf("%d", &number_x);
    number_x = array[SIZE_ARRAY - 1]; //ищем граничный элемент подмассива
    
    for (i = 0; i < n_tests; i++) {
        gettimeofday(&tv, NULL);
        main_func();
        gettimeofday(&tv1, NULL);
        
        val_time = tv1.tv_sec - tv.tv_sec;
        val_time = val_time + (float)tv1.tv_usec * 1E-6 - (float)tv.tv_usec * 1E-6;

        avg_time += val_time;
    }
    avg_time /= n_tests;
    
    printf("avg_time = %f, N_THREADS = %d\n", avg_time, N_THREADS);
    
    return 0;
}
