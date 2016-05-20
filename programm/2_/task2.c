#include <stdio.h>

/*
 * Разработать подпрограмму на языке Си, выполняющую разложение числа на
 * простые множители (например, 36=3*3*2*2). Результат – последовательность множителей
 * в массиве, ограниченная 0
 */

int prime_numbers[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void decomposition(int number)
{
	int i = 0, j = 0;
	int result[100];

	while (1) {
		if ((number % prime_numbers[i]) == 0) {	
			number /= prime_numbers[i];		
			result[j] = prime_numbers[i];
			j++;
			if (number == 1) {
				result[j] = 0;
				j++;
				break;
			}
		} else {
			i++;
		}
		if (i == 10) {
			fprintf(stderr, "error: end of prime numbers!\n");
			return;
		}
	}

	printf("Result: ");
	for (i = 0; i < j; i++) {
		printf("%d ", result[i]);
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
	int number;

	printf("Input number for decomposition: ");
	scanf("%d", &number);

	decomposition(number);

	return 0;
}