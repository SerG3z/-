/*
 * Разработать подпрограмму на языке Си, выполняющую поиск двух одинаковых
 * фрагментов в строке, не содержащих пробелы и имеющих максимальную длину.
 * Результат – указатель на начало первого из них.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// rome was a greate city
// 1 divide by space
// 2 compare words
// 3 find max words

typedef struct
{
	int len;
	char string[256];
} max_same;

max_same maxString;

int str_size(char *string)
{
	int i = 0;

	while (string[i] != '\0') {
		i++;
	}
	//printf("%d\n", i);
	return i;
}

int compare_stings(char *string, char *key, char *tmp)
{
	int i = 0;
	int j = 0;

	while (string[i] != '\0')
	{
		if (string[i] != key[j]) {
			if (j == 0) { // key[] -> first letter
				i++; // no match
			} else if (key[j] == '\0') { // key[] - end of word
				tmp[j] = '\0';
				return 0;
			} else {
				tmp[j] = '\0';
				return 0;
			}
		} else {
			tmp[j] = key[j];
			i++;
			j++;
		}
	}
	tmp[j] = '\0';
	return 0;
}

void find_max_same(char str[256][256], int size)
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i != j) {
				//printf("%s | %s\n",str[i], str[j]);
				char tmp[256];
				bzero(tmp, 256);
				
				compare_stings(str[i], str[j], &tmp);
				
				if (str_size(tmp) > 1) {
					if (str_size(tmp) > maxString.len) {
						maxString.len = str_size(tmp);
						strcpy(maxString.string, tmp);		
					}
					//printf("%s\n", tmp);
				}
			}

		}
	}
}

void divide(char *str)
{
	char words[256][256]; // list of strings
	int i = 0;
	int j = 0;
	int cnt = 0;

	while (str[i] != '\0') {
		if (str[i] == ' ') {
			words[j][cnt+1] = '\0';
			j++;
			cnt = 0;
		} else {
			words[j][cnt] = str[i];
			cnt++;
		}
		i++;
	}
	words[j][cnt+1] = '\0';
	j++;

	find_max_same(words, j);
	
	printf("lenght = %d\n", maxString.len);
	printf("string = %s\n", maxString.string);
}


int main(int argc, char const *argv[])
{
	maxString.len = 0;
	bzero(maxString.string, 256);

	char string[256];

	printf("Input test string: ");
	fgets(string, 256, stdin);

	divide(string);

	return 0;
}