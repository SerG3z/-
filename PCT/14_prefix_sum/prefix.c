#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

#define ARRAY_SIZE 10
#define NUM_THREAD 2

long long TimeValue = 0;
unsigned long long time_RDTSC()
{   union ticks
    {   unsigned long long tx;
        struct dblword { long tl, th; } dw; // little endian
    } t;
    asm("rdtsc\n": "=a"(t.dw.tl), "=d"(t.dw.th));
    return t.tx;
} // for x86 only!
void time_start() { TimeValue = time_RDTSC(); }
long long time_stop() { return time_RDTSC() - TimeValue; }

int a[ARRAY_SIZE];
int global_index = 0;
int sum = 0;
pthread_mutex_t mutex1;

struct timeval tv1, tv2, dtv;
struct timezone tz;

/*void time_start() {
    gettimeofday(&tv1, &tz);
}

long time_stop() {
    gettimeofday(&tv2, &tz);
    dtv.tv_sec = tv2.tv_sec - tv1.tv_sec;
    dtv.tv_usec = tv2.tv_usec - tv1.tv_usec;
    if (dtv.tv_usec < 0) {
    dtv.tv_sec--; dtv.tv_usec+=1000000;
    }
    return dtv.tv_sec * 1000 + dtv.tv_usec / 1000;
}*/

void *slave(void *ignored)
{
    int local_index = 0, partial_sum = 0;
    do {
        pthread_mutex_lock(&mutex1);
        local_index = global_index;
        global_index++;
        pthread_mutex_unlock(&mutex1);

        if (local_index < ARRAY_SIZE)
            partial_sum += *(a + local_index);
        //printf("sum = %d, i = %d\n", sum+partial_sum, local_index);
    } while (local_index < ARRAY_SIZE);

    pthread_mutex_lock(&mutex1);
    sum += partial_sum;

    pthread_mutex_unlock(&mutex1);

    return 0;
}

int main()
{
    time_start();
    srand(time(NULL));
    int i;
    pthread_t thread[NUM_THREAD];
    pthread_mutex_init(&mutex1, NULL);

    for (i = 0; i < ARRAY_SIZE; i++)
        a[i] = rand() % ARRAY_SIZE;

    for (i = 0; i < NUM_THREAD; i++)
        if (pthread_create(&thread[i], NULL, slave, NULL) != 0)
            perror("Pthread create fails");

    for (i = 0; i < NUM_THREAD; i++)
        if (pthread_join(thread[i], NULL) != 0)
            printf("Pthread join fails");
    printf("Prefix sum num: \n");
    for (i = 0; i < ARRAY_SIZE; ++i)
    {
       printf("%d ", a[i]);
    }
    printf("\n is %d\n", sum);
    printf("Time: %lli\n", time_stop());
    /*for (i = 0; i < ARRAY_SIZE; i++) {
    printf("a[%d] = %d\n", i, a[i]);
    }*/
}
