/*
 * Разработать подпрограмму на языке Си, формирующую массив простых чисел в
 * диапазоне от 2 до заданного. Очередное простое число определяется попыткой деления
 * нацело числа на все уже накопленные простые числа.
 */
#include <stdio.h>
#include <stdlib.h>

void get_primes(int size, int *array)
{
    int j, i, cnt = 1;
    int next_prime = 3;
    
    for (i = 1; i < size; i++) {
        int flag = 0;
        //printf("check for prime %d\n", next_prime);
        for (j = 0; j < i; j++) {
            if (array[j] == 0) {
                break;
            }
            div_t output;
            //printf("---- %d, %d\n", next_prime, array[j]);
            output = div(next_prime, array[j]);
            //printf("---------- %d %d",output.quot, output.rem);
            if (output.rem != 0)  {
                flag = 0;
            } else {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            array[cnt] = next_prime;
            cnt++;
            //printf("==> is prime %d\n", next_prime);
        } else {
            //printf("==> not prime %d\n", next_prime);
        }
        next_prime++;
    }
}

int main()
{
    int i, size;
    printf("Input amount:");
    scanf("%d", &size);
    
    int array_primes[size];
    
    for (i = 0; i < size; i++) {
        array_primes[i] = 0;
    }
    
    array_primes[0] = 2;
    
    get_primes(size, array_primes);
    
    for (i = 0; i < size; i++) {
        if (array_primes[i] == 0) {
            break;
        }
        printf("%d ", array_primes[i]);
        
    }
    printf("\n");
    return 0;
}
