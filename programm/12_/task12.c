/*
 * Разработать подпрограмму на языке Си, преобразовывающую дробную часть
 * переменной типа double в строку символов (путем последовательного умножения дробной
 * части на 10).
*/

#include <stdio.h>
#include <stdlib.h>

void floatToString(char *str, float d, int digits)
{
	int _d = (int)d;
	float tmp = d - (float)_d;

	int value;

	for (int i = 0; i < digits; i++) {
		tmp *= 10;
		value = (int)tmp;	// get next digit 
	}
	sprintf(str,"%d", value);
}

int main(int argc, char const *argv[])
{
	char string[256] = {0};
	float d = 0.0;

	printf("Input: ");
	scanf("%f", &d);

	floatToString(string, d, 3);

	printf("Result: %s\n", string);
	return 0;
}