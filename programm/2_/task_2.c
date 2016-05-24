/*
 * Разработать подпрограмму на языке Си, выполняющую разложение числа на
 * простые множители (например, 36=3*3*2*2). Результат – последовательность множителей
 * в массиве, ограниченная 0
 */

#include <stdio.h>
#include <stdlib.h>

int main(){
    unsigned long N, i;
    printf("Input\n");
    scanf("%lu", &N);
    i = 2;
    while (N > 1) {
        if ((N % i) == 0) {
            printf("%lu%c", i, N > i ? '*' : '\n');
            N /= i;
            continue;
        }
        i++;
    }
    return 0;
}
