// Производитель и потребитель
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

sem_t semA; // Объявление семафора
int mas[100];

void* producer (void* argv){
    int n = 0, val_sem;
    while(n != 1000){
        sem_getvalue(&semA, &val_sem);
        if (val_sem <= 100) {
            mas[n % 100] = rand() % 10 + 1;
            printf("произвел %d под номером %d\n", mas[n % 100], n % 100);
            n++;
            sem_post(&semA); //увеличение на 1 семафора semA
        }
        usleep((((rand() % 100) + 1) * 1000));
    }
    return NULL;
}

void* consumer (void* argv){
    int n = 0, res = 0;
    while(1){
        sem_wait(&semA);  //ждет semA=1 и уменьшение на 1 семафора semA
        res += mas[n % 100];
        printf("собрал %d под номером %d и получил %d\n", mas[n % 100], n % 100, res);
        n++;
        usleep((((rand() % 100) + 1) * 1000));
    }
    return NULL;
}

int main(){
    srand(time(NULL));
    pthread_t producerA;
    pthread_t consumerA;
    sem_init(&semA, 0, 0); //pshared=0 (в Linux только 0), первоначальное значение = 0
    if(pthread_create(&producerA, NULL, producer, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    if(pthread_create(&consumerA, NULL, consumer, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    if(pthread_join(producerA, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }
    printf("БЫЛО ДОБАВЛЕНО 1000 ЭЕЛЕМЕНТОВ\n");
    return 0;
}
