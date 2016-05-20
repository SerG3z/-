/*
 * Разработать подпрограмму на языке Си, выполняющую поиск наименьшего
 * общего кратного для всех элементов массива - минимальное число, которое делится на все
 * элементы массива без остатка
 */

#include <stdio.h>

void NOK(int *array, int size)
{
	int NOK = 0;
	int max = array[0];

	for (int i = 1; i < size; i++) {
		if (array[0] < array[i]) {
			max = array[i];
		}
	}

	printf("max %d\n", max);

	while(1) {
		int flag = 0;
		for (int i = 0; i < size; i++) {
			if (max % array[i] != 0) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			break;
		}
		max++;
	}
	printf("= %d = \n", max);
}

int main(int argc, char const *argv[])
{
	int size;
	printf("input size ");
	scanf("%d", &size);

	int array[size];

	for (int i = 0; i < size; i++) {
		scanf("%d", &array[i]);
	}

	NOK(array, size);

	return 0;
}