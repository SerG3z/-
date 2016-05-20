/*
 * Разработать подпрограмму на языке Си, выполняющую поиск двух одинаковых
 * фрагментов в строке, не содержащих пробелы и имеющих максимальную длину.
 * Результат – указатель на начало первого из них.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

typedef struct
{
	int len;
	char string[256];
} max_same;

max_same maxString;

void fun(char *key, char *string)
{
	char result[256];
	bzero(result, 256);

	char *ptr = strstr(string, key);
	
	if (ptr == NULL) {
		return;
	}

	for (int i = 0; i < strlen(key); i++) {
		result[i] = *ptr;
		ptr++;
	}

	if (strlen(key) > maxString.len) {
		maxString.len = strlen(key);
		strcpy(maxString.string, result);		
	}
}

char *find_same(char *str)
{
	int len = strlen(str);
	char tmp[len/2];
	int j = 0;

	for (int i = 0; i < (len/2); i++) {
		int cnt = i + 1;
		if (str[i] == ' ') {
			bzero(tmp, (len/2));
			j = 0;
			continue;
		} else {
			tmp[j] = str[i];
			
			char comapre[256];
			
			for (int k = 0; k < strlen(str) - i; k++) {
				comapre[k] = str[cnt];
				cnt++;
			}
			fun(tmp, comapre);

			j++;
		}
	}


	return NULL;
}

int main(int argc, char const *argv[])
{
	maxString.len = 0;
	bzero(maxString.string, 256);

	char string[256];

	printf("Input test string: ");
	fgets(string, 256, stdin);

	char *ptr;

	ptr = find_same(string);

	printf("lenght = %d\n", maxString.len);
	printf("string = %s\n", maxString.string);
	return 0;
}