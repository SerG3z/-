/*
 * Разработать подпрограмму на языке Си, осуществляющую двоичный поиск
 * строки по образцу в упорядоченном массиве строк. Если найдено несколько строк, то
 * требуется найти первую. Функция возвращает указатель на найденную строку
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 8

int BinarySearch(char *A[], char *key)
{
	int left = 0, right = N - 1, mid = 0;

	while (left <= right) {
		mid = left + (right - left) / 2; // work with index

		if (strcmp(key, A[mid]) < 0) {
			right = mid - 1;
		} else if (strcmp(key, A[mid]) > 0) { 
			left = mid + 1;
		} else {
			return mid;
		}
	}
	return -1;
}

int main(int argc, char const *argv[])
{
	char *A[N];
	char str1[256] = "a";
	char str2[256] = "a";
	char str3[256] = "ab";
	char str4[256] = "ab";
	char str5[256] = "abc";
	char str6[256] = "abc";
	char str7[256] = "abcd";
	char str8[256] = "abcde";

	A[0] = str1;
	A[1] = str2;
	A[2] = str3;
	A[3] = str4;
	A[4] = str5;
	A[5] = str6;
	A[6] = str7;
	A[7] = str8;

	char key[256] = "abc";
	
	int index = BinarySearch(A, key);

	if (index != -1) {
		printf("%d-%s\n", index, A[index]);
	} else {
		printf("not found\n");
	}

	return 0;
}