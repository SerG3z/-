
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define SIZE 24
#define MAXTHREADS 8
#define swap(A, a, b) {int tmp; tmp = A[a]; A[a] = A[b]; A[b] = tmp;}
#define sep_on_one_thread SIZE/MAXTHREADS

int A[SIZE];
int partition(int *A, int low, int high, int pivot_index);
void read();
void *QS(void *arg);
void quicksort(int *A, int low, int high);
void print();

pthread_t pt[MAXTHREADS];

int main(int argc, char *argv[])
{
    // double begin, end;
    int i, rc;
    read();
    print();
    if (rc = pthread_create(&pt[i], NULL, *QS, (void *)i)) {
        printf("%d THREAD FAILED \n", i);
    }
    pthread_join(pt[i], NULL);
    printf("FINAL THREAD %d EXITED", i);
    print();
    return 0;
}

void *QS(void *arg) {
    int i, pivot_index, rc, start, end;
    i = (int) arg;
    printf("i = %d\n", i);
    start = 0;
    end = SIZE;
    if (start >= end)
        return NULL;
    else {
        pivot_index = 0;
        pivot_index = partition(A, start, end, pivot_index);
        if ( ((pivot_index - start) <= sep_on_one_thread) || (i == MAXTHREADS - 1) ) {
            quick_sort(A, start, pivot_index);
        }
        else if (i < MAXTHREADS) {
            i++;
            if (rc = pthread_create(&pt[i], NULL, *QS, (void *)i)) {
                printf("%d THREAD Failed", i);
            }
            pthread_join(pt[i], NULL);
        }

        if (((end - pivot_index) <= sep_on_one_thread) || (i == MAXTHREADS - 1)) {
            quick_sort(A, pivot_index, end);
        }

        else if (i < MAXTHREADS) {
            i++;
            if (rc = pthread_create(&pt[i], NULL, *QS, (void *)i)) {
            }
            pthread_join(pt[i], NULL);
            printf("%d THREAD EXITED \n", i);
        }
    }
    return NULL;
}
int partition(int *A, int low, int high, int pivot_index) {
    if (pivot_index != low) {
        swap(A, low, pivot_index)
    }
    pivot_index = low;
    low++;
    while (low <= high) {
        if (A[low] <= A[pivot_index]) {
            low++;
        }
        else if (A[high] > A[pivot_index])
            high--;
        else 
            swap(A, low, high)
    }

    if(high != pivot_index)
        swap(A, pivot_index, high)
    return high;
}

void quick_sort(int *A, int low, int high) {
    int pivot_index;
    if (low >= high)
        return;
    else {
        pivot_index = (low + high) / 2;
        pivot_index = partition(A, low, high, pivot_index);
        if (low < pivot_index)
            quick_sort(A, low, pivot_index - 1);
        if (pivot_index < high)
            quick_sort(A, pivot_index + 1, high);
    }
}

void read() {
    int i;
    for (i = 0; i < SIZE; i++) {
        A[i] = rand() % 10 + 1;
    }
}

void print() {
    int i, sector;
    sector = SIZE / MAXTHREADS;
    for (i = 0; i < SIZE; i++) {
        if (i % sector == 0)
            printf("|");
        printf(" %d ", A[i]);
    }
    printf("\n\n");
}